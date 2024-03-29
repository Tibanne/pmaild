#include "PMaildCore.hpp"
#include "PMaildServer.hpp"
#include "PMaildDomain.hpp"
#include "PMaildUser.hpp"
#include <QSettings>
#include <QStringList>
#include <QDateTime>
#include <QCoreApplication>

PMaildCore::PMaildCore(QSettings &_settings): settings(_settings) {
	// init random generator for code()
	qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

PMaildUser PMaildCore::getUser(QString login) {
	// login is in the format user@domain
	int pos = login.lastIndexOf('@');
	if (pos == -1) return PMaildUser(); // invalid domain
	QString domain = login.mid(pos+1);
	QString user = login.left(pos);

	PMaildDomain dom = getDomain(domain);
	if (dom.isNull()) return PMaildUser();
	PMaildUser ouser = dom.getUser(user);
	if (ouser.isNull()) return PMaildUser();

	return ouser;
}

bool PMaildCore::authUser(QString login, QString password) {
	return getUser(login).auth(password);
}

void PMaildCore::startDaemons() {
	settings.beginGroup("daemons");

	auto d = settings.childKeys();

	for(int i = 0; i < d.size(); i++) {
		if (daemons.contains(d.at(i))) continue;

		// parse setting
		QStringList def = settings.value(d.at(i)).toStringList();
		if (def.size() != 3) continue; // not the right number

		// TYPE,LISTEN_IP,PORT
		QHostAddress addr(def.at(1));
		quint16 port;
		bool auto_ssl = false;
		PMaildServer::PMaildServerType type;

		if (def.at(0) == "SMTP") {
			type = PMaildServer::SERVER_SMTP;
		} else if (def.at(0) == "POP3") {
			type = PMaildServer::SERVER_POP3;
		} else if (def.at(0) == "IMAP4") {
			type = PMaildServer::SERVER_IMAP4;
		} else {
			qDebug("unknown daemon type, skipping");
			continue;
		}

		if (def.at(2).at(0) == '+') {
			auto_ssl = true;
			port = def.at(2).mid(1).toInt();
		} else {
			port = def.at(2).toInt();
		}

		PMaildServer *tmp = new PMaildServer(this, type, auto_ssl);
		if (!tmp->listen(addr, port)) {
			delete tmp;
			continue;
		}
		connect(tmp, SIGNAL(destroyed(QObject*)), this, SLOT(daemonLost(QObject*)));
		daemons.insert(d.at(i), tmp);
	}

	settings.endGroup();
}

void PMaildCore::daemonLost(QObject *o) {
	for(auto i = daemons.begin(); i != daemons.end(); i++) {
		if (i.value() == o)
			daemons.erase(i);
	}
}

QByteArray PMaildCore::getHostName() {
	return settings.value("core/hostname", "localhost").toByteArray();
}

QDir PMaildCore::getSpoolPath() {
	QDir path(settings.value("core/spool", "spool").toString());
	if (!path.exists()) {
		path.mkpath(".");
	}
	path.makeAbsolute();
	return path;
}

QString PMaildCore::genMailFileName() {
	// generate the filename for a mail. 
	// Format:
	// Unix_Timestamp.Pid.Random.Hostname
	return QString::number(QDateTime::currentDateTime().toTime_t()) + "." + QString::number(QCoreApplication::applicationPid()) + "." + code(10) + "." + QString::fromUtf8(getHostName());
}

QString PMaildCore::code(int len, QString chars) {
	QString res;
	while(len-- > 0) {
		res += chars.at((quint64)qrand() * chars.size() / RAND_MAX);
	}
	return res;
}

