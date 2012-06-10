#include "PMaildDomain.hpp"
#include "PMaildCore.hpp"

PMaildDomain::PMaildDomain(PMaildCore *_parent) : QObject(_parent) {
	core = _parent;
}
