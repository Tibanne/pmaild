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

quint64 PMaildDomain::getId() const {
	return info.value("domainid").toLongLong();
}

bool PMaildDomain::isNull() const {
	return core == NULL;
}

QDir PMaildDomain::getPath() const {
	if (isNull()) return QDir();
	QDir dir = core->getSpoolPath();

	if (!dir.exists("domains")) {
		dir.mkdir("domains");
	}
	dir.cd("domains");

	QString domainid;
	domainid.sprintf("%010llu", getId());
	QString path = domainid.right(1) + "/" + domainid.right(2) + "/" + domainid;

	if (!dir.exists(path)) {
		dir.mkpath(path);
	}
	dir.cd(path);

	return dir;
}

