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
	void server_cmd_auth(const QList<QByteArray>&);
	void server_cmd_help(const QList<QByteArray>&);
	void server_cmd_starttls(const QList<QByteArray>&);
	void server_cmd_mail(const QList<QByteArray>&);
	void server_cmd_rcpt(const QList<QByteArray>&);
	void server_cmd_data(const QList<QByteArray>&);
	void server_cmd_txlg(const QList<QByteArray>&);

protected:
	void handleUnknownCommand();
	void welcome();

	enum { NORMAL, AUTH_LOGIN_USERNAME, AUTH_LOGIN_PASSWORD, AUTH_PLAIN, MAIL_DATA } smtp_input_mode;
	QByteArray auth_data_username, auth_data_password;

	PMaildMtaTxn *txn;

	void parseInBuffer();

private:
	void handleAuthLoginComplete();
	void handleAuthPlainComplete(const QByteArray &);
};

