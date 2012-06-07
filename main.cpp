#include <QCoreApplication>
#include "PMaildServer.hpp"
#include "PMaildCore.hpp"

// set this define to zero once tests finished
#define SERVER_PORT_OFFSET 20000

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	// load various daemons
	PMaildCore *core = new PMaildCore();
	PMaildServer *tmp;

	tmp = new PMaildServer(core, PMaildServer::SERVER_POP3);
	tmp->listen(QHostAddress::Any, SERVER_PORT_OFFSET + 110);

	tmp = new PMaildServer(core, PMaildServer::SERVER_POP3S);
	tmp->listen(QHostAddress::Any, SERVER_PORT_OFFSET + 995);

	tmp = new PMaildServer(core, PMaildServer::SERVER_SMTP);
	tmp->listen(QHostAddress::Any, SERVER_PORT_OFFSET + 25);

	tmp = new PMaildServer(core, PMaildServer::SERVER_SMTP);
	tmp->listen(QHostAddress::Any, SERVER_PORT_OFFSET + 587);

	tmp = new PMaildServer(core, PMaildServer::SERVER_SMTPS);
	tmp->listen(QHostAddress::Any, SERVER_PORT_OFFSET + 465);

	tmp = new PMaildServer(core, PMaildServer::SERVER_IMAP4);
	tmp->listen(QHostAddress::Any, SERVER_PORT_OFFSET + 143);

	tmp = new PMaildServer(core, PMaildServer::SERVER_IMAP4S);
	tmp->listen(QHostAddress::Any, SERVER_PORT_OFFSET + 993);

	return app.exec();
}

