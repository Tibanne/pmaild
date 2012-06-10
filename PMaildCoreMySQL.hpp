#include <QObject>
#include "PMaildCore.hpp"

class QSettings;

class PMaildCoreMySQL: public PMaildCore {
	Q_OBJECT;
public:
	PMaildCoreMySQL(QSettings &settings);
	QByteArray getHostName();
	bool authUser(QString login, QString password);

	static bool check();
};

