#include <QObject>

class PMaildCore;

class PMaildMtaTxn: public QObject {
	Q_OBJECT;
public:
	PMaildMtaTxn(PMaildCore*, QObject *parent = 0);

	bool setHelo(const QByteArray&);
	const QByteArray &getHelo() const;

	void reset();

private:
	QByteArray helo;
	PMaildCore *core;
};
