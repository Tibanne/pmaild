#include "PMaildServerSmtp.hpp"
#include "PMaildCore.hpp"
#include "PMaildServer.hpp"
#include "PMaildMtaTxn.hpp"

PMaildServerSmtp::PMaildServerSmtp(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) :
	PMaildServerBase(_sock, _core, _server)
{
	txn = new PMaildMtaTxn(core, this);
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
	writeLine("250-SIZE 104857600"); // 100MB
	if (isSsl()) {
		writeLine("250-AUTH PLAIN LOGIN");
		writeLine("250-AUTH=PLAIN LOGIN");
	} else {
		writeLine("250-STARTTLS");
	}
	writeLine("250 8BITMIME");
}

