#include "PMaildMtaTxn.hpp"
#include "PMaildCore.hpp"

PMaildMtaTxn::PMaildMtaTxn(PMaildCore *_core, QObject *parent): QObject(parent) {
	core = _core;
}

bool PMaildMtaTxn::setHelo(const QByteArray&h) {
	if (h.isEmpty()) return false;
	if (!helo.isEmpty()) return false; // already got a HELO

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

bool PMaildMtaTxn::addTo(const QByteArray &to) {
	if (to.isEmpty()) {
		error_msg = "550 Target is missing";
		return false;
	}
	// TODO
	return true;
}

bool PMaildMtaTxn::prepare() {
	return true;
}

void PMaildMtaTxn::append(const QByteArray &) {
	// TODO
}

bool PMaildMtaTxn::finish() {
	error_msg = "450 Daemon not fully written yet";
	return false;
}

const QMap<QByteArray,QByteArray> &PMaildMtaTxn::transmitLog() const {
	return transmit_log;
}
