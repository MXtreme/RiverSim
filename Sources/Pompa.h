#ifndef POMPA_H
#define POMPA_H

#include "stdafx.h"
#include "Elemento.h"

class Pompa : public Elemento{
protected:
	double portata;
	float x, y;

public:
	Pompa(std::string, double, std::vector<std::string>, int, float, float, int);
	void setPortata(double);
	void setNomePompa(std::string);
	double getPortata();
	float getX();
	float getY();
	void setX(float);
	void setY(float);
};

#endif
