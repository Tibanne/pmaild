#ifndef PMAILDSERVERIMAP4_HPP
#define PMAILDSERVERIMAP4_HPP

#include "PMaildServerBase.hpp"

class PMaildServerImap4 : public PMaildServerBase
{
	Q_OBJECT;
public:
	explicit PMaildServerImap4(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

public slots:
	void server_cmd_logout(const QList<QByteArray>&);

protected:
	void handleUnknownCommand();
	void welcome();
};

#endif // PMAILDSERVERIMAP4_HPP
