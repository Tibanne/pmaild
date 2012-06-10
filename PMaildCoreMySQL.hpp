#include <QObject>
#include "PMaildCore.hpp"

class PMaildCoreMySQL: public PMaildCore {
	Q_OBJECT;
public:
	PMaildCoreMySQL();
	QByteArray getHostName();
	bool authUser(QString login, QString password);

	static bool check();
};

