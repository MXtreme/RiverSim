#include "Elemento.h"

Elemento::Elemento(std::string n){
	name = n;
}

std::string Elemento::getName(){
	return name;
}

std::vector<std::string> Elemento::getNext(){
	return next;
}

void Elemento::setName(std::string s){
	name = s;
}

int Elemento::getOrder(){
	return ordine;
}

int Elemento::getType(){
	return tipo;
}
