#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariantMap>
#include "PMaildCore.hpp"

class QSettings;

class PMaildCoreMySQL: public PMaildCore {
	Q_OBJECT;
public:
	PMaildCoreMySQL(QSettings &settings);
	PMaildDomain getDomain(QString domain);
	PMaildUser getUser(const PMaildDomain &, QString user);
	PMaildMail getEmailByUserId(const PMaildUser&user, int id);
	QList<PMaildMail> listEmailsByUserFolder(const PMaildUser&, int folder);
	bool eraseMail(const PMaildMail&);

	static bool check();

private:
	QSqlDatabase db;

	QVariantMap execQueryGetFirst(QSqlQuery &query, const QVariantMap &params = QVariantMap());

	// prepared queries ready to run
	QSqlQuery query_get_domain_by_domain;
	QSqlQuery query_get_domain_by_domainid;
	QSqlQuery query_get_domainalias;
};

