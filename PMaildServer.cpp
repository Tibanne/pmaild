#include "PMaildServer.hpp"
#include "PMaildServerSmtp.hpp"
#include "PMaildCore.hpp"
#include <QSslSocket>

PMaildServer::PMaildServer(PMaildCore *_core, PMaildServerType _type) {
	core = _core;
	type = _type;
	connect(core, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
}

void PMaildServer::incomingConnection(int socketDescriptor) {
	QSslSocket *sock = new QSslSocket(this);
	sock->setSocketDescriptor(socketDescriptor);

	switch(type) {
		case SERVER_SMTP:
			(new PMaildServerSmtp(sock, core, this))->noSsl();
			break;
	}
}

