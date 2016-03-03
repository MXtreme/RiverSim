//
// Created by eugenio on 04/01/16.
//

#ifndef CANALI_CANALE_H
#define CANALI_CANALE_H

#include <vector>
#include "stdafx.h"

class Canale{

public:
	Canale(std::string, double, double, double, std::vector<std::string>);
	
	std::string name;
	std::vector<std::string> adj;
	
    /* Timestep in secondi */
    double dt = 0.5;
    /* Lunghezza sezione in metri */
    double dx = 1.0;

    /* Parametro che indica quanto deve essere implicito l'algoritmo:
     * 0.0 Esplicito
     * 1.0 Implicito
     * Consigliato 0.6
     */
    double p_theta = 0.6;

    /* Coefficiente (?) */
    double p_beta = 1.25;
    /* Coefficiente di Manning */
    double nmann = 0.03;

    /* Larghezza canale in metri */
    double B = 1.;
    /* Pendenza fondo del canale in gradi */
    double alpha = 0.0;

    /* Gravità */
    double g = 9.8065;

    /* Sezioni in cui dividere il canale */
    int N = 50;

    /* Vettori che contengono lo stato al timestep successivo k */
    std::vector<double> Qk;
    std::vector<double> hk;

    /* Vettori che contengono lo stato al timestep attuale j */
    std::vector<double> Qj;
    std::vector<double> hj;

    /* Calcolo delle equazioni di continuità e momento basate sullo stato attuale e successivo */
    double eq_C(int i, int j);
    double eq_M(int i, int j);

    /* Flusso laterale */
    double f_q(int i, int k);

    /* Calcolo area */
    double f_A(double &d, int l);

    /* Frizione */
    double f_Sf(int i, double &d, double &d1, double &d2, double &d3);

    /* Gravità */
    double f_Se(int i, double &d, double &d1, double &d2, double &d3);

    /* Angolo in radianti */
    double f_alpha(int i);

    /* Calcolo delle equazioni dato lo stato al tempo k e al tempo j */
    std::vector<double> fun(std::vector<double> Qk, std::vector<double> hk,
                            std::vector<double> Qj, std::vector<double> hj,
                            int j);
};


#endif //CANALI_CANALE_H
