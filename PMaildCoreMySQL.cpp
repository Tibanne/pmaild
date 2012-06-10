#include "PMaildCoreMySQL.hpp"

QByteArray PMaildCoreMySQL::getHostName() {
	return "localhost";
}

bool PMaildCoreMySQL::authUser(QString login, QString password) {
	return true; // TODO code me!
}
