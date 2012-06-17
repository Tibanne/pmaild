#include "PMaildMail.hpp"

PMaildMail::PMaildMail(PMaildCore *_core, const PMaildUser &_user, const QVariantMap &_info) {
	core = _core;
	user = _user;
	info = _info;
}

PMaildMail::PMaildMail() {
	core = NULL;
}

quint64 PMaildMail::getId() const {
	if (isNull()) return 0;
	return info.value("mailid").toLongLong();
}

bool PMaildMail::isNull() const {
	return core == NULL;
}

bool PMaildMail::isDeleted() const {
	if (isNull()) return false;

	if (!flags.contains("deleted")) return false;
	return flags.value("deleted");
}

quint64 PMaildMail::getSize() const {
	if (isNull()) return 0;

	return info.value("size").toLongLong();
}

QString PMaildMail::getUniqName() const {
	if (isNull()) return QString();
	return info.value("uniqname").toString();
}

