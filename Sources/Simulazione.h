//
// Created by eugenio on 05/01/16.
//

#ifndef CANALI_SIMULAZIONE_H
#define CANALI_SIMULAZIONE_H


#include <cmath>
#include "stdafx.h"
#include "Nodo.h"
#include "Canale.h"

extern std::mutex lock;
extern bool ready;
extern std::mutex m;
extern std::condition_variable cv;
class Simulazione {


public:
	std::thread *t_sim;
	bool finish = false;

	Simulazione(){}
	~Simulazione(){
		t_sim->detach();
		//t_sim->join();
	}

	void start(){
		t_sim = new  std::thread(&Simulazione::run, this);
	}

    /* Array di lavoro necessario a hybrd
     * Deve essere lungo almeno (n*(n*3+13))/2
     * dove n è il numero di variabili del sistema.
     * Viene calcolato e allocato nella funzione init
     */
    int lwa;
    double *wa = nullptr;

    /* Vettori che contengono gli input e output alla funzione di ricerca degli zeri */
    std::vector<double> x;
    std::vector<double> fvec;

    /* Struttura per i parametri passati alla funzione valutata da hybrd.
     * xj sono i valori del sistema al time-step precedente
     * sim sarà un puntatore all'oggetto simulazione (è void perché non ho ancora finito
     * di definire la classe Simulazione)
     */
    struct params_t {
        std::vector<double> xj;
        void* sim;
    };
    /* Lo creo */
    params_t p;

    /* Tolleranza nella ricerca degli zeri */
    int tot_len;
    /* Calcola la minima tolleranza possibile, ma forse è troppo piccola questa */
    double tol = sqrt(__cminpack_func__(dpmpar)(1));

    /* Elenco dei canali e dei nodi
     * TODO: forse è meglio usare una lista invece che un vettore?
     */
    std::vector<Canale> canali;
    std::vector<Nodo>   nodi;

    /* Funzioni per calcolare x da Q e h e viceversa */
    std::vector<double> get_x();
    std::vector<double> get_Q(int i, std::vector<double> x) ;
    std::vector<double> get_h(int i, std::vector<double> x) ;

    /* Funzione valutata per trovare i residui del sistema */
    int fcn(void *p, int n, const double *x, double *fvec, int iflag);

    /* Inizializzazione */
    void init();

    /* Step nella simulazione */
    int step();
	 void set_x();
	 void run();

};


#endif //CANALI_SIMULAZIONE_H
