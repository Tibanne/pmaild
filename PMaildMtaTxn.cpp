#include "PMaildMtaTxn.hpp"
#include "PMaildCore.hpp"

PMaildMtaTxn::PMaildMtaTxn(PMaildCore *_core, QObject *parent): QObject(parent) {
	core = _core;
}

bool PMaildMtaTxn::setHelo(const QByteArray&h) {
	if (h.isEmpty()) return false;

	helo = h;
	return true;
}

bool PMaildMtaTxn::hasHelo() const {
	return !helo.isEmpty();
}

void PMaildMtaTxn::fullReset() {
	reset();
	helo = QByteArray();
}

void PMaildMtaTxn::reset() {
}

const QByteArray &PMaildMtaTxn::getHelo() const {
	return helo;
}

bool PMaildMtaTxn::setUser(const QByteArray &login, const QByteArray &pass) {
	if (!core->authUser(QString::fromUtf8(login), QString::fromUtf8(pass)))
		return false;

	auth_user_login = login;
	return true;
}

const QByteArray &PMaildMtaTxn::errorMsg() const {
	return error_msg;
}

bool PMaildMtaTxn::setFrom(const QByteArray &from, const QMap<QByteArray, QByteArray> &) {
	email_from = from;
	return true;
}
