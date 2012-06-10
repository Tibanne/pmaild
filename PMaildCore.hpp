#include <QObject>

class PMaildCore: public QObject {
	Q_OBJECT;
public:
	virtual QByteArray getHostName() = 0;
	virtual bool authUser(QString login, QString password) = 0;
};

