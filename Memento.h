#pragma once
#include <string>
#include "Originator.h"

class Memento {
	friend class Originator;
public:
	Memento(Originator);
	void setOriginator(Originator originator);
	Originator getOriginator();
private:
	Originator originator;
};