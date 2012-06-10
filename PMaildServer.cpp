#include "PMaildServer.hpp"
#include "PMaildServerSmtp.hpp"
#include "PMaildServerImap4.hpp"
#include "PMaildServerPop3.hpp"
#include "PMaildCore.hpp"
#include <QSslSocket>

PMaildServer::PMaildServer(PMaildCore *_core, PMaildServerType _type, bool _auto_ssl) {
	core = _core;
	type = _type;
	auto_ssl = _auto_ssl;
	connect(core, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
}

void PMaildServer::incomingConnection(int socketDescriptor) {
	QSslSocket *sock = new QSslSocket(this);
	sock->setSocketDescriptor(socketDescriptor);

	PMaildServerBase *i;

	switch(type) {
		case SERVER_SMTP:
			i = new PMaildServerSmtp(sock, core, this);
			break;
		case SERVER_IMAP4:
			i = new PMaildServerImap4(sock, core, this);
			break;
		case SERVER_POP3:
			i = new PMaildServerPop3(sock, core, this);
			break;
		default:
			sock->disconnect();
			sock->deleteLater();
			return;
	}

	if (auto_ssl) {
		i->ssl();
	} else {
		i->noSsl();
	}
}

