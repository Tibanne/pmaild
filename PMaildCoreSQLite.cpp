#include "PMaildCoreSQLite.hpp"
#include "PMaildDomain.hpp"
#include "PMaildUser.hpp"
#include "PMaildMail.hpp"
#include <QSqlDatabase>
#include <QSettings>
#include <QSqlError>

PMaildCoreSQLite::PMaildCoreSQLite(QSettings &settings): PMaildCore(settings) {
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(settings.value("sqlite/file", "pmaild.db").toString());
	if (!db.open()) {
		qDebug("Fatal error while initializing database backend, giving up!");
		qDebug("Error: %s", qPrintable(db.lastError().text()));
		exit(1); // ensure we exit *now*, since we are before app.exec(), calling QCoreApplication::exit() won't work
	}
}

PMaildUser PMaildCoreSQLite::getUser(const PMaildDomain &, QString) {
	return PMaildUser();
}

bool PMaildCoreSQLite::check() {
	if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
		qDebug("QSQLITE driver not available, giving up");
		return false;
	}
	return true;
}

PMaildDomain PMaildCoreSQLite::getDomain(QString) {
	return PMaildDomain();
}

QList<PMaildMail> PMaildCoreSQLite::listEmailsByUserFolder(const PMaildUser&, int) {
	return QList<PMaildMail>();
}

PMaildMail PMaildCoreSQLite::getEmailByUserId(const PMaildUser&, int) {
	return PMaildMail();
}

bool PMaildCoreSQLite::eraseMail(const PMaildMail&) {
	return false;
}

