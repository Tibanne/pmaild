#ifndef PMAILDCORE_HPP
#define PMAILDCORE_HPP

#include <QObject>
#include <QMap>

class QSettings;
class PMaildServer;
class PMaildDomain;

class PMaildCore: public QObject {
	Q_OBJECT;

public:
	PMaildCore(QSettings &settings);

	void startDaemons();

	virtual QByteArray getHostName();
	virtual bool authUser(QString login, QString password) = 0;
	virtual PMaildDomain *getDomain(QString domain) = 0;
	QSettings &settings;

public slots:
	void daemonLost(QObject*);

private:
	QMap<QString,PMaildServer*> daemons;
};

#endif /* PMAILDCORE_HPP */
