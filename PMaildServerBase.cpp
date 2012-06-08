#include "PMaildServerBase.hpp"
#include <QMetaObject>

PMaildServerBase::PMaildServerBase(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) {
	sock = _sock;
	core = _core;
	server = _server;

	connect(sock, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
}

void PMaildServerBase::socketReadyRead() {
	buf_in += sock->readAll();
	parseInBuffer();
}

void PMaildServerBase::parseInBuffer() {
	// default parseInBuffer() handler: split buffer per line, take first line, pass to parseInCommand
	while(true) {
		int pos = buf_in.indexOf('\n');
		if (pos == -1) break; // no full line in buffer
		QByteArray cmd = buf_in.left(pos).trimmed();
		buf_in.remove(0, pos+1);
		parseInCommand(cmd);
		// TODO: check if socket still valid.
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

