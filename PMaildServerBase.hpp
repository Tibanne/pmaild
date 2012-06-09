#include <QObject>
#include <QSslSocket>

class PMaildCore;
class PMaildServer;

class PMaildServerBase: public QObject {
	Q_OBJECT;
public:
	PMaildServerBase(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

public slots:
	void socketReadyRead();

protected:
	QSslSocket *sock;
	PMaildCore *core;
	PMaildServer *server;

	QByteArray buf_in;
	QByteArray buf_out;

	virtual void parseInBuffer();
	virtual void parseInCommand(const QByteArray &cmd);

	virtual void handleUnknownCommand() = 0;
};

