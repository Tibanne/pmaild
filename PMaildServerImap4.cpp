#include "PMaildServerImap4.hpp"
#include "PMaildCore.hpp"
#include <QDateTime>

PMaildServerImap4::PMaildServerImap4(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) :
	PMaildServerBase(_sock, _core, _server)
{
}

void PMaildServerImap4::welcome() {
	writeLine("* OK "+core->getHostName()+" IMAP4rev1 2001.305/pMaild on "+QDateTime::currentDateTimeUtc().toString(Qt::ISODate).toUtf8());
}

void PMaildServerImap4::handleUnknownCommand() {
	writeLine("* BAD Unknown command"); // TODO replace * with ref
}

void PMaildServerImap4::server_cmd_logout(const QList<QByteArray> &) {
	writeLine("* BYE "+core->getHostName()+" IMAP4rev1 server says bye!");
	close();
}
