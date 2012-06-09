#include "PMaildServerBase.hpp"

class PMaildServerSmtp: public PMaildServerBase {
	Q_OBJECT;

public:
	PMaildServerSmtp(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

public slots:
	void server_cmd_quit(const QList<QByteArray>&);

protected:
	void handleUnknownCommand();
	void welcome();
};

