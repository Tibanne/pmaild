#include "PMaildServerSmtp.hpp"
#include "PMaildCore.hpp"
#include "PMaildServer.hpp"
#include "PMaildMtaTxn.hpp"

PMaildServerSmtp::PMaildServerSmtp(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) :
	PMaildServerBase(_sock, _core, _server)
{
	txn = new PMaildMtaTxn(core, this);
	smtp_input_mode = NORMAL;
}

void PMaildServerSmtp::parseInBuffer() {
	while(true) {
		if (status != VALID) break;

		int pos = buf_in.indexOf('\n');
		if (pos == -1) break; // no full line in buffer
		QByteArray cmd = buf_in.left(pos).trimmed();
		buf_in.remove(0, pos+1);
		if (!cmd.isEmpty()) {
			switch(smtp_input_mode) {
				case NORMAL:
					parseInCommand(cmd);
					break;
				case AUTH_LOGIN_USERNAME:
					auth_data_username = cmd;
					smtp_input_mode = AUTH_LOGIN_PASSWORD;
					writeLine("334 "+QByteArray("Password:").toBase64());
					break;
				case AUTH_LOGIN_PASSWORD:
					auth_data_password = cmd;
					smtp_input_mode = NORMAL;
					handleAuthLoginComplete();
					break;
				case AUTH_PLAIN:
					smtp_input_mode = NORMAL;
					handleAuthPlainComplete(cmd);
					break;
			}
		}
	}
	// TODO: implement buf_in size check here, fail if buffer too big (protect against infinitely increasing buffer)
}

void PMaildServerSmtp::handleUnknownCommand() {
	writeLine("500 5.5.1 Unknown command");
}

void PMaildServerSmtp::welcome() {
	writeLine("220 "+core->getHostName()+" ESMTP (pmaild v3.0.0)");
}

void PMaildServerSmtp::server_cmd_quit(const QList<QByteArray>&) {
	writeLine("221 2.0.0 "+core->getHostName()+" closing control connexion.");
	close();
}

void PMaildServerSmtp::server_cmd_expn(const QList<QByteArray>&) {
	writeLine("502 5.5.1 EXPN and VRFY are not available");
}

void PMaildServerSmtp::server_cmd_vrfy(const QList<QByteArray>&) {
	writeLine("502 5.5.1 EXPN and VRFY are not available");
}

void PMaildServerSmtp::server_cmd_rset(const QList<QByteArray>&) {
	txn->reset();
	writeLine("250 RSET done");
}

void PMaildServerSmtp::server_cmd_noop(const QList<QByteArray>&) {
	writeLine("250 2.0.0 Ok");
}

void PMaildServerSmtp::server_cmd_helo(const QList<QByteArray>&a) {
	if ((a.isEmpty()) || (!txn->setHelo(a.at(0)))) {
		writeLine("500 5.5.2 Invalid HELO, please retry...");
		return;
	}
	writeLine("250 "+core->getHostName()+" pleased to meet you, "+txn->getHelo());
}

void PMaildServerSmtp::server_cmd_ehlo(const QList<QByteArray>&a) {
	if ((a.isEmpty()) || (!txn->setHelo(a.at(0)))) {
		writeLine("500 5.5.2 Invalid HELO, please retry...");
		return;
	}
	writeLine("250-"+core->getHostName()+" pleased to meet you, "+txn->getHelo());
	writeLine("250-PIPELINING");
	writeLine("250-ENHANCEDSTATUSCODES");
	writeLine("250-ETRN");
	writeLine("250-TXLG");
	writeLine("250-SIZE 104857600"); // 100MB TODO: fetch max mail size from config
	if (isSsl()) {
		writeLine("250-AUTH PLAIN LOGIN");
		writeLine("250-AUTH=PLAIN LOGIN");
	} else {
		writeLine("250-STARTTLS");
	}
	writeLine("250 8BITMIME");
}

// AUTH: http://www.technoids.org/saslmech.html
void PMaildServerSmtp::server_cmd_auth(const QList<QByteArray>&a) {
	if (!isSsl()) {
		writeLine("550 5.7.0 SSL required before using AUTH");
		return;
	}
	if (a.isEmpty()) {
		writeLine("550 5.5.2 Please provide a auth method");
		return;
	}
	QByteArray method = a.at(0).toLower();
	if (method == "login") {
		// at this point, we need to send 334 "Username:" (base64 encoded), then 334 "Password:" (base64 encoded too)
		writeLine("334 "+QByteArray("Username:").toBase64());
		smtp_input_mode = AUTH_LOGIN_USERNAME;
		return;
	}
	if (method == "plain") {
		if (a.size() < 2) {
			writeLine("334");
			smtp_input_mode = AUTH_PLAIN;
			return;
		}
		handleAuthPlainComplete(a.at(1));
		return;
	}
	writeLine("550 5.5.2 Unsupported auth method");
}

void PMaildServerSmtp::handleAuthLoginComplete() {
	// we got auth_data_username & auth_data_password populated
	if (!txn->setUser(QByteArray::fromBase64(auth_data_username), QByteArray::fromBase64(auth_data_password))) {
		writeLine("535 5.7.0 Authentification failed");
		return;
	}
	writeLine("235 2.0.0 OK Authenticated");
}

void PMaildServerSmtp::handleAuthPlainComplete(const QByteArray &d) {
	QList<QByteArray> l = d.split(0);
	if (l.size() != 3) {
		writeLine("550 5.5.2 Syntax error in AUTH PLAIN data");
		return;
	}
	if (!l.at(0).isEmpty()) {
		writeLine("550 5.5.2 Syntax error in AUTH PLAIN data");
		return;
	}
	if (!txn->setUser(l.at(1), l.at(2))) {
		writeLine("535 5.7.0 Authentification failed");
		return;
	}
	writeLine("235 2.0.0 OK Authenticated");
}

void PMaildServerSmtp::server_cmd_help(const QList<QByteArray>&) {
	writeLine("214 https://github.com/Tibanne/pmaild");
}

void PMaildServerSmtp::server_cmd_starttls(const QList<QByteArray>&) {
	if (isSsl()) {
		writeLine("500 STARTTLS only available in PLAIN mode. An encryption mode is already enabled");
		return;
	}
	writeLine("220 Ready to start TLS");
	txn->fullReset();
	sock->startServerEncryption();
}

void PMaildServerSmtp::server_cmd_mail(const QList<QByteArray>&a) {
	// MAIL FROM: <>
	// MAIL FROM:foo@bar.com
	// MAIL FROM:<foo@bar.com> SIZE=123456
	// MAIL FROM:
	if (!txn->hasHelo()) {
		writeLine("503 Sorry, can't let you send mails without EHLO/HELO first");
		return;
	}
	if ((a.isEmpty()) || (a.at(0).left(5).toUpper() != "FROM:")) {
		qDebug("got a(0)=%s", a.at(0).data());
		writeLine("550 Invalid syntax. Expected MAIL FROM: <user@domain.tld>");
		return;
	}

	QByteArray from;
	int pos = 0;
	if (a.at(0).size() > 5) {
		from = a.at(0).mid(5).trimmed();
	} else if (a.size() > 1) {
		from = a.at(1);
		pos++;
	}
	if ((from.at(0) == '<') && (from.right(1) == ">")) {
		from = from.mid(1, from.length()-2).trimmed();
	}
	pos++;
	QMap<QByteArray,QByteArray> meta;
	for(; pos < a.size(); pos++) {
		// meta should be in format NAME=value
		int epos = a.at(pos).indexOf('=');
		if (epos == -1) continue; // no value?
		meta.insert(a.at(pos).left(epos).toLower(), a.at(pos).mid(epos+1));
	}

	if (meta.contains("size")) {
		if (meta.value("size").toInt() > (100*1024*1024)) { // 100MB TODO: fetch max mail size from config
			writeLine("550 5.5.0 Mail too large for this system");
			return;
		}
	}

	if (!txn->setFrom(from, meta)) {
		writeLine(txn->errorMsg());
		return;
	}
	writeLine("250 2.1.0 Originator <"+from+"> OK");
}
