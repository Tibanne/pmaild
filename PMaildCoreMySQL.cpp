#include "PMaildCoreMySQL.hpp"
#include "PMaildDomain.hpp"
#include "PMaildUser.hpp"
#include "PMaildMail.hpp"
#include <QSettings>
#include <QtSql>

PMaildCoreMySQL::PMaildCoreMySQL(QSettings &settings): PMaildCore(settings) {
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(settings.value("mysql/hostname", "localhost").toString());
	db.setUserName(settings.value("mysql/username", "root").toString());
	db.setPassword(settings.value("mysql/password", "").toString());
	db.setDatabaseName(settings.value("mysql/database", "pmaild").toString());
	if (!db.open()) {
		qDebug("Fatal error while initializing database backend, giving up!");
		qDebug("Error: %s", qPrintable(db.lastError().text()));
		exit(1); // ensure we exit *now*, since we are before app.exec(), calling QCoreApplication::exit() won't work
	}

	query_get_domain_by_domain = QSqlQuery(db);
	query_get_domain_by_domain.prepare("SELECT * FROM domains WHERE domain = :domain");
	query_get_domain_by_domainid = QSqlQuery(db);
	query_get_domain_by_domainid.prepare("SELECT * FROM domains WHERE domainid = :domainid");
	query_get_domainalias = QSqlQuery(db);
	query_get_domainalias.prepare("SELECT * FROM domainaliases WHERE domain = :domain");
}

bool PMaildCoreMySQL::check() {
	if (!QSqlDatabase::isDriverAvailable("QMYSQL")) {
		qDebug("QMYSQL driver not available, giving up");
		return false;
	}
	return true;
}

PMaildDomain PMaildCoreMySQL::getDomain(QString domain) {
	// get domain from db
	QMap<QString,QString> params;
	params.insert(":domain", domain);

	QVariantMap data = execQueryGetFirst(query_get_domain_by_domain, params);
	if (data.isEmpty()) {
		// check for domain aliases
		data = execQueryGetFirst(query_get_domainalias, params);
		if (data.isEmpty()) return PMaildDomain();

		params.clear();
		params.insert(":domainid", data.value("domainid").toString());
		data = execQueryGetFirst(query_get_domain_by_domainid, params);
		if (data.isEmpty()) return PMaildDomain();
	}

	return PMaildDomain(this, data);
}

PMaildUser PMaildCoreMySQL::getUser(const PMaildDomain &domain, QString user) {
	QSqlQuery q(db);
	q.prepare(QString("SELECT * FROM z%1_accounts WHERE user = :user").arg(domain.getId()));

	QMap<QString,QString> params;
	params.insert(":user", user);

	QVariantMap res = execQueryGetFirst(q, params);
	if (res.isEmpty()) return PMaildUser();

	return PMaildUser(this, domain, res);
}

QVariantMap PMaildCoreMySQL::execQueryGetFirst(QSqlQuery &query, const QMap<QString,QString> &params) {
	for(auto i = params.begin(); i != params.end(); i++)
		query.bindValue(i.key(), i.value());
	
	if (!query.exec()) {
		qDebug("MySQL error: %s", qPrintable(query.lastError().text()));
		return QVariantMap();
	}

	if ((!query.isActive()) || (!query.isSelect()) || (!query.first())) {
		return QVariantMap();
	}

	QSqlRecord rec = query.record();
	QVariantMap res;

	for(int i = 0; i < rec.count(); i++) {
		res.insert(rec.fieldName(i), query.value(i));
	}

	query.finish();
	return res;
}

QList<PMaildMail> PMaildCoreMySQL::listEmailsByUserFolder(const PMaildUser&user, int folder) {
	QSqlQuery q(db);
	q.prepare(QString("SELECT * FROM z%1_mails WHERE userid = :userid AND folder = :folder").arg(user.getDomain().getId()));
	q.bindValue(":userid", user.getId());
	q.bindValue(":folder", folder);

	if (!q.exec()) {
		qDebug("MySQL error: %s", qPrintable(q.lastError().text()));
		return QList<PMaildMail>();
	}
	if ((!q.isActive()) || (!q.isSelect()) || (!q.first()))
		return QList<PMaildMail>();
	
	QSqlRecord rec = q.record();
	QList<PMaildMail> res;

	do {
		QVariantMap tmp;
		for(int i = 0; i < rec.count(); i++)
			tmp.insert(rec.fieldName(i), q.value(i));
		res.append(PMaildMail(this, user, tmp));
	} while(q.next());

	return res;
}

