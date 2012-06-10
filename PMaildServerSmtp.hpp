#include "PMaildServerBase.hpp"

class PMaildMtaTxn;

class PMaildServerSmtp: public PMaildServerBase {
	Q_OBJECT;

public:
	PMaildServerSmtp(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

public slots:
	void server_cmd_quit(const QList<QByteArray>&);
	void server_cmd_expn(const QList<QByteArray>&);
	void server_cmd_vrfy(const QList<QByteArray>&);
	void server_cmd_rset(const QList<QByteArray>&);
	void server_cmd_noop(const QList<QByteArray>&);
	void server_cmd_helo(const QList<QByteArray>&);
	void server_cmd_ehlo(const QList<QByteArray>&);

protected:
	void handleUnknownCommand();
	void welcome();

	PMaildMtaTxn *txn;
};

