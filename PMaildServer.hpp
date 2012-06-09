#include <QTcpServer>

class PMaildCore;

class PMaildServer: public QTcpServer {
	Q_OBJECT;
public:
	enum PMaildServerType {
		SERVER_POP3,
		SERVER_POP3S,
		SERVER_IMAP4,
		SERVER_IMAP4S,
		SERVER_SMTP,
		SERVER_SMTPS
	};

	PMaildServer(PMaildCore*, PMaildServerType type);

	void incomingConnection(int socketDescriptor);

private:
	PMaildServerType type;
	PMaildCore *core;
};

