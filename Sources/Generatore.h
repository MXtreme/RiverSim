#ifndef GENERATORE_H
#define GENERATORE_H

#include "Graph.h"
#include "Parser.h"

class Generatore{
private:
	Graph map;
	Parser *p;
	std::vector<Canale> canali;
	std::vector<Nodo> pompe;
	std::vector<ElemGraph*> canali_node;
	std::vector<ElemGraph*> pompe_node;
	int pompe_cap = 0;
	bool built = false;

public:
	Generatore(Parser *p);
	void build(std::string name_file);
	Graph* getMap();
	std::vector<Canale> getCanali();
	std::vector<Nodo> getPompe();
	bool getBuilt();
};

#endif