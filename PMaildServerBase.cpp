#include "PMaildServerBase.hpp"

PMaildServerBase::PMaildServerBase(QSslSocket *_sock, PMaildCore *_core, PMaildServer *_server) {
	sock = _sock;
	core = _core;
	server = _server;
}
