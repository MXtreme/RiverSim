#ifndef ELEMGRAPH_H
#define ELEMGRAPH_H

#include "stdafx.h"
#include "Nodo.h"

class ElemGraph{
protected:
	int id;
	int type;
	Nodo *e;
	std::vector<Canale*> next;

public:
	ElemGraph(Nodo*, int);
	int getNAdj();
	std::vector<Canale*> getNext();
	int getId();
	bool isPompa();
	bool isCanale();
	Nodo getElemento();
	Nodo* getElementPointer();
	void setNext(Canale*);
};

#endif
