#include "PMaildServerPop3.hpp"
#include "PMaildCore.hpp"

PMaildServerPop3::PMaildServerPop3(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) :
	PMaildServerBase(_sock, _core, _server)
{
}

void PMaildServerPop3::welcome() {
	writeLine("+OK "+core->getHostName()+" POP3 (pmaild v3.0.0)");
}

void PMaildServerPop3::handleUnknownCommand() {
	writeLine("-ERR Unknown command");
}

void PMaildServerPop3::server_cmd_quit(const QList<QByteArray>&) {
	writeLine("+OK "+core->getHostName()+" closing control connexion.");
	close();
}
