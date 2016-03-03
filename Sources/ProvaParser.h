#include "Parser.h"
#include "ElemGraph.h"
#include "Elemento.h"
#include "Nodo.h"
#include "Canale.h"
#include <vector>
#include <iostream>

class ProvaParser : public Parser{

protected:
	std::string line, tmp;
	std::vector<std::string> next;
	int count, offset;
	char *temp;
	std::vector<std::string> estraiAdiacenti(std::string);
	std::vector<Elemento> newElement;

public:
	void readFile(std::vector<Canale>*, std::vector<Nodo>*, std::string);


};