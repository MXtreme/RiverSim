#ifndef ELEMENTO_H
#define ELEMENTO_H

#include "stdafx.h"

class Elemento{
protected:
	int tipo, ordine;
	std::string name;
	std::vector<std::string> next;

public:
	Elemento(std::string);
	std::string getName();
	void setName(std::string);
	std::vector<std::string> getNext();
	int getOrder();
	int getType();
};

#endif
