//
// Created by eugenio on 04/01/16.
//

#include "Nodo.h"

Nodo::Nodo(std::string nome, double portata, double altezza, int x, int y, std::vector<std::string> adiac, int nodo_type){
	name = nome;
	Q_set = portata;
	h_set = altezza;
	x_pos = x;
	y_pos = y;
	adj = adiac;
	visitato = false;
	type = nodo_type;
/*
	printf("NODO %s %f %f %d %d %d ", name.c_str(), Q_set, h_set, x_pos, y_pos, type);
printf("\n");
	for(int i=0;i<adj.size();i++){
		printf("ADIACENTI %s ",adj[i].c_str());
	}
printf("\n");
	for(int i=0;i<canali_in.size();i++){
		printf("CANALI_IN %d ", canali_in[i]);
	}
printf("\n");
	for(int i=0;i<canali_out.size();i++){
		printf("CANALI_OUT %d ", canali_in[i]);
	}
	printf("\n");
*/
}

std::vector<double> Nodo::fun(std::vector<Canale> canali) {
    std::vector<double> ret;
    int i;

    switch (type) {
        /* Caso di altezza fissata
         * L'ultimo valore di tutti i canali in ingresso deve essere uguale all'altezza fissata
         * Il primo valore di tutti i canali in uscita deve essere uguale all'altezza fissata
         */
        case Nodo::fixed_h: {
/*
				printf("CANALI_IN_SIZE %d\n", canali_in.size());
				printf("CANALI_OUT_SIZE %d\n", canali_out.size());				
*/
            for (i = 0; i < canali_in.size(); ++i) {
                ret.push_back(h_set - canali[canali_in[i]].hk.back());
            }
            for (i = 0; i < canali_out.size(); ++i) {
                ret.push_back(h_set - canali[canali_out[i]].hk.front());
            }

            break;
        }

        /* Caso di portata fissata
         * L'ultimo valore di tutti i canali in ingresso deve essere uguale alla portata fissata
         * Il primo valore di tutti i canali in uscita deve essere uguale alla portata fissata
         */
        case Nodo::fixed_Q: {
            for (i = 0; i < canali_in.size(); ++i) {
                ret.push_back( Q_set - canali[canali_in[i]].Qk.back() );
            }
            for (i = 0; i < canali_out.size(); ++i) {
                ret.push_back( Q_set - canali[canali_out[i]].Qk.front() );
            }
            break;
        }

        /* Caso di giunzione tra più canali
         * La somma algebrica delle portate deve essere nulla, dove i canali in ingresso son presi col segno
         * meno e qulli in uscita col segno più.
         * Le altezze devono essere tutte uguali, quindi si creano N-1 condizioni tra le coppie di altezze,
         * con N numero totale di canali in entrata e uscita dal nodo.
         */
        case Nodo::junction: {
            double sumQ = 0.;
            for (i = 0; i < canali_in.size(); ++i) {
                sumQ -= canali[canali_in[i]].Qk.back();
            }
            for (i = 0; i < canali_out.size(); ++i) {
                sumQ += canali[canali_out[i]].Qk.front();
            }
            ret.push_back(sumQ);

            std::vector<double> hs;
            for (i = 0; i < canali_in.size(); ++i) {
                hs.push_back(canali[canali_in[i]].hk.back());
            }
            for (i = 0; i < canali_out.size(); ++i) {
                hs.push_back(canali[canali_out[i]].hk.front());
            }

            for (i = 0; i < hs.size() - 1; ++i) {
                ret.push_back(hs[i] - hs[i + 1]);
            }
            break;
        }
        default: {
            break;
        }
    }

    return ret;
}

void Nodo::setVisitato(bool b){
	visitato = b;
}

bool Nodo::getVisitato(){
	return visitato;
}
