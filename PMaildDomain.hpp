#ifndef PMAILDDOMAIN_HPP
#define PMAILDDOMAIN_HPP

#include <QVariantMap>
#include <QDir>

class PMaildCore;
class PMaildUser;

class PMaildDomain {
public:
	PMaildDomain(PMaildCore *parent, const QVariantMap &info);
	PMaildDomain();

	PMaildUser getUser(QString user);
	bool authUser(QString user, QString password);

	quint64 getId() const;
	QDir getPath() const;

	bool isNull() const;

private:
	PMaildCore *core;
	QVariantMap info;
};

#endif // PMAILDDOMAIN_HPP
