#include "PMaildDomain.hpp"
#include "PMaildUser.hpp"
#include "PMaildCore.hpp"

PMaildDomain::PMaildDomain() {
	core = NULL;
}

PMaildDomain::PMaildDomain(PMaildCore *_parent, const QVariantMap &_info) {
	core = _parent;
	info = _info;
}

PMaildUser PMaildDomain::getUser(QString user) {
	if (isNull()) return PMaildUser();
	return core->getUser(*this, user);
}

int PMaildDomain::getId() const {
	return info.value("domainid").toInt();
}

bool PMaildDomain::isNull() const {
	return core == NULL;
}

