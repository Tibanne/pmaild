#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslConfiguration>
#include "PMaildCoreMySQL.hpp"
#include "PMaildCoreSQLite.hpp"

static bool init_ssl(QSettings &settings);

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QSettings settings("pmaild.ini", QSettings::IniFormat);

	if (!init_ssl(settings)) return 1;

	// initialize core
	PMaildCore *core;
	QString core_type = settings.value("core/backend", "SQLITE").toString().toUpper();

	if (core_type == "MYSQL") {
		if (!PMaildCoreMySQL::check()) {
			qDebug("MySQL driver not available in this build of Qt");
			return 2;
		}
		core = new PMaildCoreMySQL(settings);
	} else if (core_type == "SQLITE") {
		if (!PMaildCoreSQLite::check()) {
			qDebug("SQLite3 driver not available in this build of Qt");
			return 2;
		}
		core = new PMaildCoreSQLite(settings);
	} else {
		qDebug("Invalid core backend selected, please fix configuration");
		return 2;
	}

	core->startDaemons();

	return app.exec();
}

static bool init_ssl(QSettings &settings) {
	// init SSL
	settings.beginGroup("ssl");
	QFile key_file(settings.value("key", "ssl/server.key").toString());
	if (!key_file.open(QIODevice::ReadOnly)) {
		qDebug("Could not open SSL key file");
		return false;
	}
	QSslKey ssl_key = QSslKey(&key_file, QSsl::Rsa);
	if (ssl_key.isNull()) {
		qDebug("Failed to read SSL key");
		return false;
	}
	key_file.close();
	QSslCertificate ssl_cert = QSslCertificate::fromPath(settings.value("crt", "ssl/server.crt").toString(), QSsl::Pem, QRegExp::FixedString).at(0);

	if (ssl_cert.isNull()) {
		qDebug("failed to locate certificate");
		return false;
	}

	QSslConfiguration config = QSslConfiguration::defaultConfiguration();
	config.setLocalCertificate(ssl_cert);
	config.setPrivateKey(ssl_key);
	QSslConfiguration::setDefaultConfiguration(config);

	settings.endGroup();

	return true;
}
