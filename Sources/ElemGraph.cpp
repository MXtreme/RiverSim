#include "ElemGraph.h"

ElemGraph::ElemGraph(Nodo *el, int n){
	e = el;
	id = n;
}

int ElemGraph::getNAdj(){
	return next.size();
}

std::vector<Canale*> ElemGraph::getNext(){
	return next;
}

int ElemGraph::getId(){
	return id;
}

Nodo ElemGraph::getElemento(){
	return *e;
}

Nodo* ElemGraph::getElementPointer(){
	return e;
}

void ElemGraph::setNext(Canale *n){
	next.push_back(n);
}
