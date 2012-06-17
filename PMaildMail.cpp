#include "PMaildMail.hpp"

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

