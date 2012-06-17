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

	QList<PMaildMail> listEmailsByFolder(int folder = 0);

private:
	QVariantMap info;
	PMaildDomain domain;
	PMaildCore *core;
};

#endif /* PMAILDUSER_HPP */
