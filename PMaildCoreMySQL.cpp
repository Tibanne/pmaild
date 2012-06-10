#include "PMaildCoreMySQL.hpp"
#include <QSqlDatabase>

PMaildCoreMySQL::PMaildCoreMySQL(QSettings &settings): PMaildCore(settings) {
}

QByteArray PMaildCoreMySQL::getHostName() {
	return "localhost";
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
