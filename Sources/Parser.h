#ifndef PARSER_H 
#define PARSER_H

#include <fstream>
#include <string>
#include <stdlib.h>
#include "ElemGraph.h"
#include "Canale.h"
#include "Nodo.h"

class Parser{

protected:
	float lunghezza, larghezza, sezione, pendenza, velocita, portata, altezza, x, y;
	std::string nome;
	int tipo, ordine, tipo_pompa;;

public:
	virtual void readFile(std::vector<Canale>*, std::vector<Nodo>*, std::string) = 0;
};

#endif