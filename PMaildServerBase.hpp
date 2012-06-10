#ifndef PMAILDSERVERBASE_HPP
#define PMAILDSERVERBASE_HPP

#include <QObject>
#include <QSslSocket>
#include <QByteArray>
#include <QTimer>
#include <QSslError>

class PMaildCore;
class PMaildServer;

class PMaildServerBase: public QObject {
	Q_OBJECT;
public:
	PMaildServerBase(QSslSocket *sock, PMaildCore *core, PMaildServer *server);

	void write(const QByteArray&);
	void writeLine(const QByteArray&);
	void flush();
	void close();
	void setTimeout(int ms);
	bool isSsl();
	void noSsl();
	void ssl();

public slots:
	void socketReadyRead();
	void socketBytesWritten(qint64);
	void socketDisconnected();
	void socketSslReady();
	void socketSslErrors(const QList<QSslError>&);
	void timeout();

protected:
	QSslSocket *sock;
	PMaildCore *core;
	PMaildServer *server;

	QTimer timer;

	QByteArray buf_in;
	QByteArray buf_out;

	virtual void parseInBuffer();
	virtual void parseInCommand(const QByteArray &cmd);

	virtual void handleUnknownCommand() = 0;

	virtual void welcome() = 0;

	enum { INIT, VALID, CLOSING } status;
};

#endif /* PMAILDSERVERBASE_HPP */
