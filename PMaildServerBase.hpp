#include <QObject>
#include <QSslSocket>

class PMaildCore;
class PMaildServer;

class PMaildServerBase: public QObject {
	Q_OBJECT;
public:
	PMaildServerBase(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

protected:
	QSslSocket *sock;
	PMaildCore *core;
	PMaildServer *server;
};

