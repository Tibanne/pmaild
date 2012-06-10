#include <QObject>
#include <QMap>

class PMaildCore;

class PMaildMtaTxn: public QObject {
	Q_OBJECT;
public:
	PMaildMtaTxn(PMaildCore*, QObject *parent = 0);

	bool setHelo(const QByteArray&);
	const QByteArray &getHelo() const;
	bool hasHelo() const;

	void reset();
	void fullReset(); // for STARTTLS

	bool setUser(const QByteArray &login, const QByteArray &pass);
	bool setFrom(const QByteArray &from, const QMap<QByteArray,QByteArray>&meta);

	const QByteArray &errorMsg() const;

private:
	QByteArray helo;
	QByteArray auth_user_login;
	QByteArray email_from;

	QByteArray error_msg;
	PMaildCore *core;
};
