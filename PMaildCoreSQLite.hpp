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
	bool authUser(QString login, QString password);

	static bool check();

private:
	QSqlDatabase db;
};

#endif // PMAILDCORESQLITE_HPP
