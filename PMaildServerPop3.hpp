#ifndef PMAILDSERVERPOP3_HPP
#define PMAILDSERVERPOP3_HPP

#include "PMaildServerBase.hpp"

class PMaildServerPop3 : public PMaildServerBase
{
	Q_OBJECT;
public:
	explicit PMaildServerPop3(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

public slots:
	void server_cmd_quit(const QList<QByteArray>&);

protected:
	void handleUnknownCommand();
	void welcome();
};

#endif // PMAILDSERVERPOP3_HPP
