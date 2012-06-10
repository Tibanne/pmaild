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

void PMaildMtaTxn::reset() {
}

const QByteArray &PMaildMtaTxn::getHelo() const {
	return helo;
}

