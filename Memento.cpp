#include "Memento.h"

Memento::Memento(Originator newOriginator): originator(newOriginator){}

void Memento::setOriginator(Originator originator) {
	this->originator = originator;
}

Originator Memento::getOriginator() { return this->originator; }