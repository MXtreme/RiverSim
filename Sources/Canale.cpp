//
// Created by eugenio on 04/01/16.
//

#include <cmath>
#include "Canale.h"
#include <iostream>

Canale::Canale(std::string nome, double lunghezza, double larghezza, double pendenza, std::vector<std::string> adiac){
	name = nome;
	dx = lunghezza;
	B = larghezza;
	alpha = pendenza;
	adj = adiac;
}

double Canale::eq_C(int i, int j) {

    int l = i + 1;
    int k = j + 1;

    return p_theta * (Qk[l] - Qk[i] - dx * f_q(i, k)) +
           (1 - p_theta) * (Qj[l] - Qj[i] - dx * f_q(i, j)) +
           dx / 2. / dt * (f_A(hk[l], l) + f_A(hk[i], i) - f_A(hj[l], l) - f_A(hj[i], i));
}


double Canale::eq_M(int i, int j) {

    int l = i + 1;
    int k = j + 1;

    double Sfik = f_Sf(i, Qk[i], Qk[l], hk[i], hk[l]);
    double Seik = f_Se(i, Qk[i], Qk[l], hk[i], hk[l]);

    double Sfij = f_Sf(i, Qj[i], Qj[l], hj[i], hj[l]);
    double Seij = f_Se(i, Qj[i], Qj[l], hj[i], hj[l]);

	//printf("EQ_M %f %f %f %f\n ", Sfik, Seik, Sfij, Seij);

    return dx / 2. / dt * (Qk[l] + Qk[i] - Qj[l] - Qj[i]) +
           p_theta * (p_beta * Qk[l] * Qk[l] / f_A(hk[l], l) - p_beta * Qk[i] * Qk[i] / f_A(hk[i], i) +
           g * (f_A(hk[i], i) + f_A(hk[l], l)) / 2. * (hk[l] - hk[i] + dx * Sfik + dx * Seik)) +
           (1 - p_theta) * (p_beta * Qj[l] * Qj[l] / f_A(hj[l], l) - p_beta * Qj[i] * Qj[i] / f_A(hj[i], i) +
           g * (f_A(hj[i], i) + f_A(hj[l], l)) / 2. * (hj[l] - hj[i] + dx * Sfij + dx * Seij));

}


std::vector<double> Canale::fun(std::vector<double> _Qk, std::vector<double> _hk, std::vector<double> _Qj, std::vector<double> _hj, int j) {
    /* Assegna i dati dell'algoritmo allo stato del sistema così da poter
     * chiamare le funzioni di continuità e di momento senza parametri
     */
	
	//printf("Qk_begin %f Qk_end \n", _Qk.begin(), _Qk.end());
	//printf("hk_begin %f hk_end \n", _hk.begin(), _hk.end());
		
    Qk.assign(_Qk.begin(), _Qk.end());
    hk.assign(_hk.begin(), _hk.end());
    Qj.assign(_Qj.begin(), _Qj.end());
    hj.assign(_hj.begin(), _hj.end());
/*
	printf("Qk:");
	for(int i=0; i<Qk.size(); i++){
		printf("FAIL %f ", Qk[i]);
	}
	printf("\n");

	printf("hk:");
	for(int i=0; i<hk.size(); i++){
		printf("%f ", hk[i]);
	}
	printf("\n");

	printf("Qj:");
	for(int i=0; i<Qj.size(); i++){
		printf("%f ", Qj[i]);
	}
	printf("\n");

	printf("hj:");
	for(int i=0; i<hj.size(); i++){
		printf("%f ", hj[i]);
	}
	printf("\n");
*/
    /* Inizializza e riempi vettore di ritorno (sappiamo che è lungo 2*(N-1)) */
    std::vector<double> ret((unsigned long)(2*(N-1)),0.);
    for (int i = 0; i < N-1; ++i) {
        ret[i]   = eq_C(i,j);
        ret[N-1+i] = eq_M(i,j);
    }

    return ret;
}

double Canale::f_q(int i, int k) {
    /* Nessun flusso laterale (per ora) */
    return 0;
}

double Canale::f_A(double &h, int l) {
    /* Sezione rettangolare (per ora) */
    return h * B;
}

double Canale::f_Sf(int i, double &Qi, double &Ql, double &hi, double &hl) {
    int l = i + 1;
    double Q_ = (Qi + Ql) / 2.;
    double A_ = (f_A(hi, i) + f_A(hl, l)) / 2.;

    double R = B * hi / (B + 2 * hi);  // https://it.wikipedia.org/wiki/Raggio_idraulico#Rettangolare
    return nmann * nmann * std::abs(Q_) * Q_ / (A_ * A_) / std::pow(R, 4. / 3.);

}

double Canale::f_Se(int i, double &Qi, double &Ql, double &hi, double &hl) {
    /* Trova il seno dell'angolo, trasformando in radianti */
    return std::sin(M_PI/180.*f_alpha(i));
}

double Canale::f_alpha(int i) {
    return -alpha;
}
