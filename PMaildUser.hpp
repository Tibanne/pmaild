#ifndef PMAILDUSER_HPP
#define PMAILDUSER_HPP

#include <QVariantMap>
#include "PMaildDomain.hpp"

class PMaildCore;
class PMaildDomain;
class PMaildMail;

class PMaildUser {
public:
	PMaildUser(PMaildCore*core, const PMaildDomain&, const QVariantMap &);
	PMaildUser();

	bool auth(QString password);

	bool isNull() const;
	int getId() const;

	PMaildMail getEmailById(quint64 id);
	QList<PMaildMail> listEmailsByFolder(int folder = 0);

	PMaildDomain getDomain();
	const PMaildDomain &getDomain() const;

private:
	QVariantMap info;
	PMaildDomain domain;
	PMaildCore *core;
};

#endif /* PMAILDUSER_HPP */
