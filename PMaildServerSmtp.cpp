#include "PMaildServerSmtp.hpp"
#include "PMaildCore.hpp"
#include "PMaildServer.hpp"

PMaildServerSmtp::PMaildServerSmtp(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) :
	PMaildServerBase(_sock, _core, _server)
{
	// void
}

void PMaildServerSmtp::handleUnknownCommand() {
	qDebug("unknown cmd");
}

void PMaildServerSmtp::server_cmd_quit(const QList<QByteArray>&) {
	qDebug("quit");
	sock->close();
}

void PMaildServerSmtp::welcome() {
	writeLine("220 "+core->getHostName()+" ESMTP (pmaild v3.0.0)");
}

