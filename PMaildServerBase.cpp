#include "PMaildServerBase.hpp"
#include "PMaildServer.hpp"
#include "PMaildCore.hpp"
#include <QMetaObject>

PMaildServerBase::PMaildServerBase(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) {
	sock = _sock;
	core = _core;
	server = _server;

	connect(sock, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(sock, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
	connect(this, SIGNAL(destroyed(QObject*)), sock, SLOT(deleteLater()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
	connect(core, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));
	connect(server, SIGNAL(destroyed(QObject*)), this, SLOT(deleteLater()));

	status = INIT;

	// timeout for receiving stuff
	timer.setSingleShot(true);
	timer.start(60000);
}

void PMaildServerBase::noSsl() {
	connect(sock, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
	connect(sock, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));
	sock->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

	status = VALID;

	welcome();

	// read data if any is already in the pipe
	if (sock->bytesAvailable()) socketReadyRead();
}

void PMaildServerBase::socketSslErrors(const QList<QSslError>&) {
	qDebug("ssl error, giving up");
}

void PMaildServerBase::socketSslReady() {
	connect(sock, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
	connect(sock, SIGNAL(bytesWritten(qint64)), this, SLOT(socketBytesWritten(qint64)));

	sock->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
	timer.start(60000);

	status = VALID;

	welcome();

	// read data if any is already in the pipe
	if (sock->bytesAvailable()) socketReadyRead();
}

void PMaildServerBase::socketDisconnected() {
	sock->deleteLater();
	deleteLater();
}

void PMaildServerBase::timeout() {
	if (status == CLOSING) {
		socketDisconnected();
	} else {
		close();
	}
}

void PMaildServerBase::socketBytesWritten(qint64) {
	flush();
}

void PMaildServerBase::socketReadyRead() {
	if (status != VALID) return;

	buf_in += sock->readAll();
	parseInBuffer();
}

void PMaildServerBase::parseInBuffer() {
	// default parseInBuffer() handler: split buffer per line, take first line, pass to parseInCommand
	while(true) {
		if (status != VALID) break;

		int pos = buf_in.indexOf('\n');
		if (pos == -1) break; // no full line in buffer
		QByteArray cmd = buf_in.left(pos).trimmed();
		buf_in.remove(0, pos+1);
		if (!cmd.isEmpty())
			parseInCommand(cmd);
	}
	// TODO: implement buf_in size check here, fail if buffer too big (protect against infinitely increasing buffer)
}

void PMaildServerBase::parseInCommand(const QByteArray &cmd) {
	// default parseInCommand() handler: split by spaces and lookup local cmd table
	
	QList<QByteArray> list = cmd.split(' ');

	// search list for empty elements and drop these
	for(auto i = list.begin(); i != list.end(); i++) {
		if ((*i).isEmpty()) list.erase(i);
	}

	QByteArray method = "server_cmd_" + list.at(0).toLower();
	
	if (!QMetaObject::invokeMethod(this, method.data(), Qt::DirectConnection, Q_ARG(QList<QByteArray>, list))) {
		handleUnknownCommand();
	}
}

void PMaildServerBase::close() {
	if (status == CLOSING) return;
	status = CLOSING;
	flush();
}

void PMaildServerBase::flush() {
	if (buf_out.size() == 0) {
		if (status == CLOSING) sock->disconnectFromHost();
		return;
	}
	qint64 res = sock->write(buf_out);
	if (res == -1) {
		// write error!
		buf_out.clear();
		sock->disconnectFromHost();
		sock->deleteLater();
		deleteLater();
		return;
	}
	if (res == 0) return;

	// give it up to 10 seconds to read all
	if (status == CLOSING)
		timer.start(10000);
	
	if (res == buf_out.size()) { // all written
		buf_out.clear();
		if (status == CLOSING) sock->disconnectFromHost();
		return;
	}
	buf_out.remove(0, res);
}

void PMaildServerBase::writeLine(const QByteArray &b) {
	if (status != VALID) return;
	buf_out.append(b);
	buf_out.append("\r\n");
	flush();
}

void PMaildServerBase::write(const QByteArray &b) {
	if (status != VALID) return;
	buf_out.append(b);
	flush();
}

