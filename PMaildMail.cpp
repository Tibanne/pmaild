#include "PMaildMail.hpp"
#include "PMaildCore.hpp"

PMaildMail::PMaildMail(PMaildCore *_core, const PMaildUser &_user, const QVariantMap &_info) {
	core = _core;
	user = _user;
	info = _info;
}

PMaildMail::PMaildMail() {
	core = NULL;
}

quint64 PMaildMail::getId() const {
	if (isNull()) return 0;
	return info.value("mailid").toLongLong();
}

bool PMaildMail::isNull() const {
	return core == NULL;
}

bool PMaildMail::isDeleted() const {
	if (isNull()) return false;

	if (!flags.contains("deleted")) return false;
	return flags.value("deleted");
}

quint64 PMaildMail::getSize() const {
	if (isNull()) return 0;

	return info.value("size").toLongLong();
}

QString PMaildMail::getUniqName() const {
	if (isNull()) return QString();
	return info.value("uniqname").toString();
}

QString PMaildMail::getFilePath() const {
	QDir dir = user.getPath();
	return dir.filePath(getUniqName());
}

bool PMaildMail::erase() {
	if (!core->eraseMail(*this)) return false;

	QFile f(getFilePath());
	f.remove();
	return true;
}

PMaildUser PMaildMail::getUser() {
	return user;
}

const PMaildUser PMaildMail::getUser() const {
	return user;
}

QByteArray PMaildMail::readAll() {
	QFile f(getFilePath());
	if (!f.open(QIODevice::ReadOnly)) return QByteArray();

	return f.readAll(); // ~QFile will close it
}

QByteArray PMaildMail::readLines(int lines) {
	// limit to header + lines lines
	QByteArray res;
	QFile f(getFilePath());
	if (!f.open(QIODevice::ReadOnly)) return QByteArray();

	bool headers = true;
	while(true) {
		QByteArray line = f.readLine();
		if (line == "") break;

		if (headers) {
			res.append(line);
			if (line.trimmed() == "")
				headers = false;
			continue;
		}

		if (--lines <= 0) break;
		res.append(line);
	}
	return res;
}

bool PMaildMail::unsetFlag(const QString &) {
	// TODO
	return false;
}

