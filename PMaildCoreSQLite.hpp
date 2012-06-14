#ifndef PMAILDCORESQLITE_HPP
#define PMAILDCORESQLITE_HPP

#include <QObject>
#include <QSqlDatabase>
#include "PMaildCore.hpp"

class QSettings;

class PMaildCoreSQLite: public PMaildCore {
	Q_OBJECT;
public:
	PMaildCoreSQLite(QSettings &settings);
	PMaildDomain getDomain(QString domain);
	PMaildUser getUser(const PMaildDomain&, QString user);

	static bool check();

private:
	QSqlDatabase db;
};

#endif // PMAILDCORESQLITE_HPP
