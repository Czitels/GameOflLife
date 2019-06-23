#include "Originator.h"
#include "Memento.h"

Originator::Originator(const std::string& RLE): RLE(RLE){}

void Originator::setRLE(const std::string& RLE) { this->RLE = RLE; }
const std::string Originator::getRLE()const { return this->RLE; }

std::unique_ptr<Memento> Originator::createMemento() {
	return std::make_unique<Memento>(*this);
}
void Originator::restoreToMemento(std::unique_ptr<Memento> memento) {
	*this = memento->getOriginator();
}