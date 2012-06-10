#include "PMaildCoreSQLite.hpp"
#include <QSqlDatabase>

PMaildCoreSQLite::PMaildCoreSQLite(QSettings &settings): PMaildCore(settings) {
	db = QSqlDatabase::addDatabase("SQLITE");
}

bool PMaildCoreSQLite::authUser(QString login, QString password) {
	return false;
}

bool PMaildCoreSQLite::check() {
	if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
		qDebug("QSQLITE driver not available, giving up");
		return false;
	}
	return true;
}
