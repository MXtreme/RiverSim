//
// Created by eugenio on 04/01/16.
//

#ifndef CANALI_NODO_H
#define CANALI_NODO_H


#include <vector>
#include "Canale.h"
#include "stdafx.h"

class Nodo{
public:
	static const int fixed_h = 0;
	static const int fixed_Q = 1;
	static const int junction = 2;

   int type;

	Nodo(std::string, double, double, int, int, std::vector<std::string>, int nodo_type);

	bool visitato;
	std::string name;
	int x_pos, y_pos;
	std::vector<std::string> adj;
	
    /* Elenco degli indici dei canali in ingresso e in uscita */
    std::vector<int> canali_in;
    std::vector<int> canali_out;

    /* Valori impostati per le tipologie fisse */
    double h_set;
    double Q_set;

    /* Calcolo delle funzioni del nodo */
    std::vector<double> fun(std::vector<Canale> canali);

	void setVisitato(bool);
	bool getVisitato();
};

#endif //CANALI_NODO_H
