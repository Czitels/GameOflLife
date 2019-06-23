#include <memory>
#include "Caretaker.h"
#include <iostream>
#include <iterator>

Caretaker::Caretaker(){}

void::Caretaker::setMemento(std::unique_ptr<Memento> memento) {
	listMemento.push_back(std::move(memento));
}

std::unique_ptr<Memento> Caretaker::getMemento() {
	std::unique_ptr<Memento> lastElem;
	lastElem = std::move(listMemento.back());
	listMemento.pop_back();
	return lastElem;
}

void Caretaker::clearList() {
	this->listMemento.clear();
}

void Caretaker::eraseList(size_t position) {
	std::list<std::unique_ptr<Memento>>::iterator itr = listMemento.begin();
	std::advance(itr, position);
	listMemento.erase(listMemento.begin(), itr);
}
