#pragma once
#include "Memento.h"
#include <list>
class Caretaker {
public:
	Caretaker();

	void setMemento(std::unique_ptr<Memento>);
	std::unique_ptr<Memento> getMemento();
	void clearList();
	void eraseList(size_t position);

private:
	std::list<std::unique_ptr<Memento>> listMemento;
};