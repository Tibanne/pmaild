#ifndef PMAILDCORE_HPP
#define PMAILDCORE_HPP

#include <QObject>
#include <QMap>
#include <QDir>

class QSettings;
class PMaildServer;
class PMaildDomain;
class PMaildUser;
class PMaildMail;

class PMaildCore: public QObject {
	Q_OBJECT;

public:
	PMaildCore(QSettings &settings);

	void startDaemons();

	virtual QByteArray getHostName();
	bool authUser(QString login, QString password);
	PMaildUser getUser(QString login);
	QDir getSpoolPath();
	QString genMailFileName();
	QString code(int len, QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"); // generate random string of length "int"

	virtual PMaildDomain getDomain(QString domain) = 0;
	virtual PMaildUser getUser(const PMaildDomain&, QString user) = 0;
	virtual PMaildMail getEmailByUserId(const PMaildUser&user, int id) = 0;
	virtual QList<PMaildMail> listEmailsByUserFolder(const PMaildUser&, int folder = 0) = 0;
	virtual bool eraseMail(const PMaildMail&) = 0; // remove mail from db
	QSettings &settings;

public slots:
	void daemonLost(QObject*);

private:
	QMap<QString,PMaildServer*> daemons;
};

#endif /* PMAILDCORE_HPP */
