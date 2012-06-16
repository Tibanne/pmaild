#include "PMaildUser.hpp"
#include <unistd.h> /* for crypt() */

PMaildUser::PMaildUser(PMaildCore*_core, const PMaildDomain&_domain, const QVariantMap &_info) {
	core = _core;
	domain = _domain;
	info = _info;
}

PMaildUser::PMaildUser() {
	core = NULL;
}

bool PMaildUser::isNull() const {
	return core == NULL;
}

bool PMaildUser::auth(QString password) {
	if (isNull()) return false;
	// convert to QByteArray for unix
	// salt => [a–zA–Z0–9./]
	QByteArray p = password.toUtf8();
	QByteArray db_pass = info.value("password").toString().toUtf8();
	if (db_pass.left(3) == "$1$") {
		// standard crypt password
		// http://qt-project.org/doc/qt-4.8/qbytearray.html#QByteArray-2
		// QByteArray makes a deep copy of the string data.
		QByteArray c(crypt(p, db_pass));
		if (c == db_pass) return true;
		qDebug("failed login, good pass=%s bad pass=%s", db_pass.data(), c.data());
		return false;
	}
	qDebug("Unknown DB password type: %s", qPrintable(info.value("password").toString()));
	return false; // TODO
}

