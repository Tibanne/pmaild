#include "PMaildServerPop3.hpp"
#include "PMaildCore.hpp"
#include "PMaildMail.hpp"
#include <QStringList>

PMaildServerPop3::PMaildServerPop3(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) :
	PMaildServerBase(_sock, _core, _server)
{
	lid_max = 0;
}

quint64 PMaildServerPop3::getLidFromId(quint64 id) {
	if (id2lid.contains(id)) return id2lid.value(id);
	lid_max++;
	id2lid.insert(id, lid_max);
	lid2id.insert(lid_max, id);
	return lid_max;
}

void PMaildServerPop3::welcome() {
	writeLine("+OK "+core->getHostName()+" POP3 (pmaild v3.0.0)");
}

void PMaildServerPop3::handleUnknownCommand() {
	writeLine("-ERR Unknown command");
}

void PMaildServerPop3::parseInBuffer() {
	while(true) {
		if (status != VALID) break;

		int pos = buf_in.indexOf('\n');
		if (pos == -1) break; // no full line in buffer

		QByteArray cmd = buf_in.left(pos).trimmed();
		buf_in.remove(0, pos+1);
		if (!cmd.isEmpty()) {
			switch(pop3_input_mode) {
				case NORMAL:
					parseInCommand(cmd);
					break;
				case AUTH_PLAIN:
					pop3_input_mode = NORMAL;
					handleAuthPlainComplete(cmd);
					break;
			}
		}
	}
	// TODO: implement buf_in size check here, fail if buffer too big (protect against infinitely increasing buffer)
}
void PMaildServerPop3::server_cmd_quit(const QList<QByteArray>&) {
	if (!user.isNull()) {
		for(auto i = toDelete.begin(); i != toDelete.end(); i++) {
			PMaildMail mail = user.getEmailById(*i);
			if (mail.isNull()) continue;
			mail.erase();
		}
	}
	writeLine("+OK "+core->getHostName()+" closing control connexion.");
	close();
}

void PMaildServerPop3::server_cmd_stls(const QList<QByteArray>&) {
	if (isSsl()) {
		writeLine("-ERR STLS only available in PLAIN mode. An encryption mode is already enabled");
		return;
	}
	writeLine("+OK Begin TLS negotiation");
	sock->startServerEncryption();
}

void PMaildServerPop3::server_cmd_capa(const QList<QByteArray>&) {
	QStringList capa;
	capa << "TOP" << "PIPELINING" << "UIDL" << "IMPLEMENTATION pMaild 3.0";

	if (isSsl()) {
		capa << "USER" << "SASL PLAIN"; // SASL CRAM-MD5 DIGEST-MD5 PLAIN
	} else {
		capa << "STLS";
	}

	writeLine("+OK");
	for(int i = 0; i < capa.size(); i++) writeLine(capa.at(i).toUtf8());
	writeLine(".");
}

void PMaildServerPop3::server_cmd_noop(const QList<QByteArray>&) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	writeLine("+OK");
}

void PMaildServerPop3::server_cmd_user(const QList<QByteArray>&p) {
	if (!user.isNull()) {
		writeLine("-ERR already logged in");
		return;
	}
	if (!isSsl()) {
		writeLine("-ERR Please STLS before logging in");
		return;
	}
	if (p.size() != 1) {
		writeLine("-ERR Syntax: USER <login>");
		return;
	}
	auth_data_username = p.at(0);
	writeLine("+OK Provide PASSword now, please");
}

void PMaildServerPop3::server_cmd_pass(const QList<QByteArray>&p) {
	if (!user.isNull()) {
		writeLine("-ERR already logged in");
		return;
	}
	if (!isSsl()) {
		writeLine("-ERR Please STLS before logging in");
		return;
	}
	if (p.size() != 1) {
		writeLine("-ERR Syntax: PASS <password>");
		return;
	}

	PMaildUser luser = core->getUser(QString::fromUtf8(auth_data_username));
	if (!luser.auth(QString::fromUtf8(p.at(0)))) {
		writeLine("-ERR Authorization denied");
		return;
	}
	user = luser;
	writeLine("+OK Authorization granted");
}

void PMaildServerPop3::server_cmd_auth(const QList<QByteArray>&p) {
	if (!user.isNull()) {
		writeLine("-ERR already logged in");
		return;
	}
	if (!isSsl()) {
		writeLine("-ERR Please STLS before logging in");
		return;
	}
	if (p.size() == 0) {
		writeLine("+OK list of SASL extensions follows");
		writeLine("PLAIN");
		writeLine(".");
		return;
	}
	if (p.at(0) != "PLAIN") {
		writeLine("-ERR Unknown AUTH method");
		return;
	}
	if (p.size() == 1) {
		writeLine("+ go ahead");
		pop3_input_mode = AUTH_PLAIN;
		return;
	}
	handleAuthPlainComplete(p.at(1));
}

void PMaildServerPop3::handleAuthPlainComplete(const QByteArray &b64) {
	QByteArray s = QByteArray::fromBase64(b64);
	if (s.at(0) != 0) {
		writeLine("-ERR syntax error");
		return;
	}
	QList<QByteArray> r = s.split(0);
	if (r.size() != 3) {
		writeLine("-ERR syntax error");
		return;
	}
	PMaildUser luser = core->getUser(QString::fromUtf8(r.at(1)));
	if (!luser.auth(QString::fromUtf8(r.at(2)))) {
		writeLine("-ERR Authorization denied");
		return;
	}
	user = luser;
	writeLine("+OK Authorization granted");
}

void PMaildServerPop3::server_cmd_rset(const QList<QByteArray>&) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	toDelete.clear();
	writeLine("+OK");
}

void PMaildServerPop3::server_cmd_stat(const QList<QByteArray>&) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	qint64 size = 0, count = 0;
	QList<PMaildMail> list = user.listEmailsByFolder(0); // Folder 0 = INBOX

	for(int i = 0; i < list.size(); i++) {
		if (list.at(i).isDeleted()) continue;
		// TODO: check local toDelete
		size += list.at(i).getSize();
		getLidFromId(list.at(i).getId()); // assign a local id
		count++;
	}
	writeLine(QString("+OK %1 %2").arg(count).arg(size).toUtf8());
}

void PMaildServerPop3::server_cmd_list(const QList<QByteArray>&p) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	if (p.size() > 0) {
		quint64 lid = p.at(0).toLongLong();
		if (!lid2id.contains(lid)) {
			writeLine("-ERR Unknown message");
			return;
		}
		quint64 id = lid2id.value(lid);
		if (toDelete.contains(id)) {
			writeLine("-ERR Message was deleted, you can still restore it using RSET");
			return;
		}
		PMaildMail mail = user.getEmailById(id);
		if (mail.isNull()) {
			writeLine("-ERR email has been lost");
			return;
		}
		writeLine(QString("+OK %1 %2").arg(getLidFromId(mail.getId())).arg(mail.getSize()).toUtf8());
		return;
	}
	QList<PMaildMail> list = user.listEmailsByFolder(0); // Folder 0 = INBOX

	writeLine("+OK");

	for(int i = 0; i < list.size(); i++) {
		// TODO: return correct mail id
		writeLine(QString("%1 %2").arg(getLidFromId(list.at(i).getId())).arg(list.at(i).getSize()).toUtf8());
	}
	writeLine(".");
}

void PMaildServerPop3::server_cmd_uidl(const QList<QByteArray>&p) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	if (p.size() > 0) {
		quint64 lid = p.at(0).toLongLong();
		if (!lid2id.contains(lid)) {
			writeLine("-ERR Unknown message");
			return;
		}
		quint64 id = lid2id.value(lid);
		if (toDelete.contains(id)) {
			writeLine("-ERR Message was deleted, you can still restore it using RSET");
			return;
		}
		PMaildMail mail = user.getEmailById(id);
		if (mail.isNull()) {
			writeLine("-ERR email has been lost");
			return;
		}
		writeLine(QString("+OK %1 %2").arg(getLidFromId(mail.getId())).arg(mail.getUniqName()).toUtf8());
		return;
	}
	QList<PMaildMail> list = user.listEmailsByFolder(0); // Folder 0 = INBOX

	writeLine("+OK");

	for(int i = 0; i < list.size(); i++) {
		// TODO: return correct mail id
		writeLine(QString("%1 %2").arg(getLidFromId(list.at(i).getId())).arg(list.at(i).getUniqName()).toUtf8());
	}
	writeLine(".");
}

void PMaildServerPop3::server_cmd_dele(const QList<QByteArray>&p) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	if (p.size() != 1) {
		writeLine("-ERR syntax error. Should use DELE <id>");
		return;
	}
	quint64 lid = p.at(0).toLongLong();
	if (!lid2id.contains(lid)) {
		writeLine("-ERR Unknown message");
		return;
	}
	quint64 id = lid2id.value(lid);
	if (toDelete.contains(id)) {
		writeLine("-ERR Message is already marked for deletion");
		return;
	}
	toDelete.insert(id);
	writeLine("+OK Nessage marked for deletion");
}

void PMaildServerPop3::server_cmd_retr(const QList<QByteArray>&p) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	if (p.size() != 1) {
		writeLine("-ERR syntax error. Should use RETR <id>");
		return;
	}
	quint64 lid = p.at(0).toLongLong();
	if (!lid2id.contains(lid)) {
		writeLine("-ERR Unknown message");
		return;
	}
	quint64 id = lid2id.value(lid);
	if (toDelete.contains(id)) {
		writeLine("-ERR Message is marked for deletion");
		return;
	}
	PMaildMail mail = user.getEmailById(id);
	if (mail.isNull()) {
		writeLine("-ERR mail is missing from server");
		return;
	}
	QByteArray data = mail.readAll();
	data = data.replace("\n.", "\n..");
	writeLine("+OK");
	write(data);
	if (data.right(1) != "\n") write("\r\n");
	writeLine(".");

	mail.unsetFlag("recent");
}

void PMaildServerPop3::server_cmd_top(const QList<QByteArray>&p) {
	if (user.isNull()) {
		writeLine("-ERR need to login first");
		return;
	}
	if ((p.size() < 1) || (p.size() > 2)) {
		writeLine("-ERR syntax error. Should use TOP <id> [lines]");
		return;
	}
	quint64 lid = p.at(0).toLongLong();
	if (!lid2id.contains(lid)) {
		writeLine("-ERR Unknown message");
		return;
	}
	quint64 id = lid2id.value(lid);
	if (toDelete.contains(id)) {
		writeLine("-ERR Message is marked for deletion");
		return;
	}
	PMaildMail mail = user.getEmailById(id);
	if (mail.isNull()) {
		writeLine("-ERR mail is missing from server");
		return;
	}
	int lines = 0;
	if (p.size() == 2) lines = p.at(1).toInt();

	QByteArray data = mail.readLines(lines);
	data = data.replace("\n.", "\n..");
	writeLine("+OK");
	write(data);
	if (data.right(1) != "\n") write("\r\n");
	writeLine(".");

	mail.unsetFlag("recent");
}

