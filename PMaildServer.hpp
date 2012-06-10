#include <QTcpServer>

class PMaildCore;

class PMaildServer: public QTcpServer {
	Q_OBJECT;
public:
	enum PMaildServerType {
		SERVER_POP3,
		SERVER_IMAP4,
		SERVER_SMTP
	};

	PMaildServer(PMaildCore*, PMaildServerType type, bool auto_ssl = false);

	void incomingConnection(int socketDescriptor);

private:
	PMaildServerType type;
	PMaildCore *core;
	bool auto_ssl;
};

