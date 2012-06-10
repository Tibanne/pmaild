#include <QObject>
#include <QSqlDatabase>
#include "PMaildCore.hpp"

class QSettings;

class PMaildCoreMySQL: public PMaildCore {
	Q_OBJECT;
public:
	PMaildCoreMySQL(QSettings &settings);
	bool authUser(QString login, QString password);
	PMaildDomain *getDomain(QString domain);

	static bool check();

private:
	QSqlDatabase db;
};

