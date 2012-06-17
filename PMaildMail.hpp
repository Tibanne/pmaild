#include <QVariantMap>
#include "PMaildUser.hpp"

class PMaildCore;
class PMaildUser;

class PMaildMail {
public:
	PMaildMail();
	PMaildMail(PMaildCore *core, const PMaildUser &user, const QVariantMap &info);

	bool isNull() const;
	quint64 getId() const;

	bool isDeleted() const; // check if flag "deleted" is set
	quint64 getSize() const; // return size of email
	QString getUniqName() const; // return uniqname

private:
	PMaildCore *core;
	PMaildUser user;
	QVariantMap info;

	QMap<QString, bool> flags;
};

