#include "Pompa.h"

Pompa::Pompa(std::string n, double p, std::vector<std::string> nt, int ord, float x_coord, float y_coord, int type) : Elemento(n){
	name = n;
	portata = p;
	next = nt;
	ordine = ord;
	x = x_coord;
	y = y_coord;
	tipo = type;
}

void Pompa::setPortata(double p){
	portata = p;
}

void Pompa::setNomePompa(std::string n){
	name = n;
}

double Pompa::getPortata(){
	return portata;
}

float Pompa::getX(){
	return x;
}

float Pompa::getY(){
	return y;
}

void Pompa::setX(float x_coord){
	x = x_coord;
}

void Pompa::setY(float y_coord){
	y = y_coord;
}
