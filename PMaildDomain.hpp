#ifndef PMAILDDOMAIN_HPP
#define PMAILDDOMAIN_HPP

#include <QObject>

class PMaildCore;

class PMaildDomain: public QObject {
	Q_OBJECT;
public:
	explicit PMaildDomain(PMaildCore *parent);

private:
	PMaildCore *core;
};

#endif // PMAILDDOMAIN_HPP
