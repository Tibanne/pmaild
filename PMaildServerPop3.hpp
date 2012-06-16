#ifndef PMAILDSERVERPOP3_HPP
#define PMAILDSERVERPOP3_HPP

#include "PMaildServerBase.hpp"
#include "PMaildUser.hpp"

class PMaildServerPop3 : public PMaildServerBase
{
	Q_OBJECT;
public:
	explicit PMaildServerPop3(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

public slots:
	void server_cmd_quit(const QList<QByteArray>&);
	void server_cmd_stls(const QList<QByteArray>&);
	void server_cmd_auth(const QList<QByteArray>&);
	void server_cmd_user(const QList<QByteArray>&);
	void server_cmd_pass(const QList<QByteArray>&);
	void server_cmd_noop(const QList<QByteArray>&);
	void server_cmd_capa(const QList<QByteArray>&);
	void server_cmd_rset(const QList<QByteArray>&);
//	void server_cmd_stat(const QList<QByteArray>&);
//	void server_cmd_list(const QList<QByteArray>&);
//	void server_cmd_uidl(const QList<QByteArray>&);
//	void server_cmd_retr(const QList<QByteArray>&);
//	void server_cmd_top(const QList<QByteArray>&);
//	void server_cmd_dele(const QList<QByteArray>&);

protected:
	void handleUnknownCommand();
	void welcome();

	enum { NORMAL, AUTH_PLAIN } pop3_input_mode;
	QByteArray auth_data_username;

	void parseInBuffer();

	PMaildUser user; // currently logged in user
	QMap<int,QString> toDelete; // map of mails pending deletion

private:
	void handleAuthPlainComplete(const QByteArray &);
};

#endif // PMAILDSERVERPOP3_HPP
