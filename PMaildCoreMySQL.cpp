#include "PMaildCoreMySQL.hpp"
#include <QSqlDatabase>
#include <QSettings>
#include <QSqlError>

PMaildCoreMySQL::PMaildCoreMySQL(QSettings &settings): PMaildCore(settings) {
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(settings.value("mysql/hostname", "localhost").toString());
	db.setUserName(settings.value("mysql/username", "root").toString());
	db.setPassword(settings.value("mysql/password", "").toString());
	db.setDatabaseName(settings.value("mysql/database", "pmaild").toString());
	if (!db.open()) {
		qDebug("Fatal error while initializing database backend, giving up!");
		qDebug("Error: %s", qPrintable(db.lastError().text()));
		exit(1); // ensure we exit *now*, since we are before app.exec(), calling QCoreApplication::exit() won't work
	}
}

bool PMaildCoreMySQL::authUser(QString login, QString password) {
	return true; // TODO code me!
}

bool PMaildCoreMySQL::check() {
	if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
		qDebug("QMYSQL driver not available, giving up");
		return false;
	}
	return true;
}

PMaildDomain *PMaildCoreMySQL::getDomain(QString domain) {
	return NULL;
}
