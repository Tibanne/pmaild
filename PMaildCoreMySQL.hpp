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
	QList<PMaildMail> listEmailsByUserFolder(const PMaildUser&, int folder);

	static bool check();

private:
	QSqlDatabase db;

	QVariantMap execQueryGetFirst(QSqlQuery &query, const QMap<QString,QString> &params = QMap<QString,QString>());

	// prepared queries ready to run
	QSqlQuery query_get_domain_by_domain;
	QSqlQuery query_get_domain_by_domainid;
	QSqlQuery query_get_domainalias;
};

