#include "PMaildServer.hpp"
#include "PMaildServerSmtp.hpp"
#include <QSslSocket>

PMaildServer::PMaildServer(PMaildCore *_core, PMaildServerType _type) {
	core = _core;
	type = _type;
}

void PMaildServer::incomingConnection(int socketDescriptor) {
	QSslSocket *sock = new QSslSocket(this);
	sock->setSocketDescriptor(socketDescriptor);

	switch(type) {
		case SERVER_SMTP:
			new PMaildServerSmtp(sock, core, this);
	}
}

