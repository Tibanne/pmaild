#include <QObject>
#include <QMap>

class QSettings;
class PMaildServer;

class PMaildCore: public QObject {
	Q_OBJECT;
public:
	PMaildCore(QSettings &settings);

	void startDaemons();

	QMap<QString,PMaildServer*> daemons;

	virtual QByteArray getHostName() = 0;
	virtual bool authUser(QString login, QString password) = 0;
	QSettings &settings;
};

