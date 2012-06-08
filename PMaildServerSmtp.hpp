#include "PMaildServerBase.hpp"

class PMaildServerSmtp: public PMaildServerBase {
	Q_OBJECT;

public:
	PMaildServerSmtp(QSslSocket *sock, PMaildCore *core, PMaildServer *server);
};

