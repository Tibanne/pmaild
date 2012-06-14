#include <QVariantMap>
#include "PMaildDomain.hpp"

class PMaildCore;
class PMaildDomain;

class PMaildUser {
public:
	PMaildUser(PMaildCore*core, const PMaildDomain&, const QVariantMap &);
	PMaildUser();

	bool auth(QString password);

	bool isNull() const;

private:
	QVariantMap info;
	PMaildDomain domain;
	PMaildCore *core;
};

