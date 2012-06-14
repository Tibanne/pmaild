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
	// convert to QByteArray for unix
	// salt => [a–zA–Z0–9./]
	QByteArray p = password.toUtf8();
	return false; // TODO
}

