#include <QObject>
#include "PMaildCore.hpp"

class QSettings;

class PMaildCoreMySQL: public PMaildCore {
	Q_OBJECT;
public:
	PMaildCoreMySQL(QSettings &settings);
	bool authUser(QString login, QString password);

	static bool check();
};

