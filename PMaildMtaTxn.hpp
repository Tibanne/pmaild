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
	bool addTo(const QByteArray &to);

	bool prepare();
	void append(const QByteArray &data);
	bool finish();

	const QByteArray &errorMsg() const;
	const QMap<QByteArray,QByteArray> &transmitLog() const;

private:
	QByteArray helo;
	QByteArray auth_user_login;
	QByteArray email_from;
	QMap<QByteArray,QByteArray> email_to;

	QMap<QByteArray,QByteArray> transmit_log;

	QByteArray error_msg;
	PMaildCore *core;
};
