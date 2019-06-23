#pragma once
#include <memory>
#include <string>

class Memento;

class Originator{
public:
	Originator()=default;
	Originator(const std::string &);

	void setRLE(const std::string &name);
	const std::string getRLE() const;

	std::unique_ptr<Memento> createMemento();
	void restoreToMemento(std::unique_ptr<Memento>);
private:
	std::string RLE;
};
