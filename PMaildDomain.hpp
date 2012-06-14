#ifndef PMAILDDOMAIN_HPP
#define PMAILDDOMAIN_HPP

#include <QVariantMap>

class PMaildCore;
class PMaildUser;

class PMaildDomain {
public:
	PMaildDomain(PMaildCore *parent, const QVariantMap &info);
	PMaildDomain();

	PMaildUser getUser(QString user);
	bool authUser(QString user, QString password);

	int getId() const;

	bool isNull() const;

private:
	PMaildCore *core;
	QVariantMap info;
};

#endif // PMAILDDOMAIN_HPP
