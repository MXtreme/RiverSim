//
// Created by eugenio on 05/01/16.
//

#include <cassert>
#include <vector>
#include "Simulazione.h"


int Simulazione::fcn(void *p, int n, const double *x, double *fvec, int iflag) {
    /* Recupero lo stato del sistema al time-step precedente j */
    std::vector<double> xj = ((params_t*)p)->xj;
    /* Mi copio l'array x nel vettore xk (stiamo stimando il time-step successivo k) */
    std::vector<double> xk(x, x+n);
    /* Alcuni vettori di ritorno utili */
    std::vector<double> ret;
    std::vector<double> ret_canale;
    std::vector<double> ret_nodo;

    /* Per ogni canale, calcola le equazioni Q e h e inserisci il risulstato in ret */
	//printf("geth  ");
    for (int i = 0; i < canali.size(); ++i) {
			//printf("%f ", get_h(i,xj));
        ret_canale = canali[i].fun(get_Q(i, xk), get_h(i, xk), get_Q(i, xj), get_h(i, xj), 0);
        ret.insert(ret.end(), ret_canale.begin(), ret_canale.end());
    }
	//printf("\n");

    /* Per ogni nodo, calcola le equazioni caratteristiche e inserisci il risultato in ret */
    for (int i = 0; i < nodi.size(); ++i) {
        ret_nodo = nodi[i].fun(canali);
        ret.insert(ret.end(), ret_nodo.begin(), ret_nodo.end());
    }

    assert(ret.size()==n);

    /* Copia il vettore ret nell'array fvec */
    std::copy(ret.begin(), ret.end(), fvec);


    ret.clear();
    ret_canale.clear();
    ret_nodo.clear();

    /* TODO: controllare cosa è opportuno restituire */
    return 0;
}


std::vector<double> Simulazione::get_x() {
    /* Crea il vettore x concatenando i Q e gli h di ciascun canale */
    std::vector<double> ret;
    for (int i = 0; i < canali.size(); ++i) {
        ret.insert(ret.end(), canali[i].Qj.begin(), canali[i].Qj.end());
        ret.insert(ret.end(), canali[i].hj.begin(), canali[i].hj.end());
    }
    return ret;
}

void Simulazione::set_x() {
    unsigned int index = 0;
    for (int i = 0; i < canali.size(); ++i) {
        for (int j = 0; j < canali[i].Qj.size(); ++j) {
            x.at(index++) = canali[i].Qj[j];
        }
        for (int j = 0; j < canali[i].hj.size(); ++j) {
            x.at(index++) = canali[i].hj[j];
        }
    }
}

std::vector<double> Simulazione::get_Q(int i, std::vector<double> x) {

    /* Calcola da che indice parte il canale i-esimo nel vettore x */
    int index = 0;
    for (int j=0; j<i; j++) {
        index += canali[j].N*2;
    }
    /* Assegna a ret i valori dei Q */
    std::vector<double> ret;
    ret.assign(x.begin()+index,x.begin()+index+canali[i].N);
	
/*
	printf("get_Q:");
	for(int i=0; i<ret.size(); i++){
		printf("%f ", ret[i]);
	}
printf("\n");
*/

    return ret;
}

std::vector<double> Simulazione::get_h(int i, std::vector<double> x) {
    /* Calcola da che indice parte il canale i-esimo nel vettore x */
    int index = 0;
    for (int j=0; j<i; j++) {
        index += canali[j].N*2;
    }
    /* Assegna a ret i valori degli h */
    std::vector<double> ret;
    ret.assign(x.begin()+index+canali[i].N, x.begin()+index+canali[i].N*2);
    return ret;
}

void Simulazione::init() {
    /* Calcola quanti segmenti in totale ci sono nel sistema descritto.
     * Il doppio di questo numero sarà pari al numero di incognite
     */
    tot_len=0;
    for (int i = 0; i < canali.size(); ++i) {
        tot_len += canali[i].N;
    }

    /* Alloca il working array in base a tot_len */
    if ( wa != nullptr ) delete[] wa;
    lwa = (tot_len*2*(3*tot_len*2+13))/2;
    wa = new double[lwa]; // no less than (n*(3*n+13))/2.

    /* Associa a p questa specifica simulazione */
    p.sim = this;

    /* Imposta una tolleranza */
    tol = 1E-4;


    /* Inizializza x */
    for (int i = 0; i < canali.size(); ++i) {
        for (int j = 0; j < canali[i].Qj.size(); ++j) {
            x.push_back(0);
            x.push_back(0);
        }
    }
    fvec.assign(x.begin(), x.end());
}

/* E' necessario uno stratagemmo per poter contenere la funzione chiamata da hybrd in una classe.
 * Infatti, hybdr prevede come primo argomento una funzione da chiamare, ma per le specifiche di c++
 * questa non può essere un membro di una classe (o per lo meno non in modo banale).
 * Perciò, ho incluso in p un puntatore all'istanza della classe Simulazione in uso, così che si possa poi accedere
 * alla specifica funzione fcn, che accede a sua volta ai dati di canali nodi e quant'altro.
 * In questo modo, la funzione fcn2 non ha alcun riferimenti alla precisa istanza di Simulazione
 * Un po' difficile da spiegare...
 */
int fcn2(void *p, int n, const double *x, double *fvec, int iflag) {
    /* Dall'interno all'esterno:
     * 1) Sia p un puntatore a un oggetto di tipo Simulazione::params_t
     * 2) Estrai l'attributo sim
     * 3) Sia quest'attributo sim un puntatore a un'oggetto di tipo Simulazione
     * 4) Dell'oggetto puntato, chiama la funzione fcn con gli stessi parametri della chiamata a questa funzione
     *    e ritornane il valore
     */
    return ((Simulazione*)(((Simulazione::params_t *) p)->sim))->fcn(p,n,x,fvec,iflag);
}

int Simulazione::step() {
    /* Calcola x e fvec (uguale a x per comodità, ma deve solamente essere lungo uguale) */
//    x = get_x();
//    fvec = get_x();
    set_x();

    /* Assegna lo stato attuale a p */
    p.xj = x;

    /* Esegui la ricerca degli zeri.
     * Come scritto prima, viene chiamata la funzione fcn2.
     * p Deve essere passato come referenza
     * I vettori x e fvec sono rappresentati in memoria come array. Si può accedere all'indirizzo
     * del primo elemento con la sitassi &vettore[0]
     */
    int info = __cminpack_func__(hybrd1)(fcn2, &p, 2*tot_len, &x[0], &fvec[0], tol, wa, lwa);

    /* Riassegna i risultati ai rispettivi canali */
    for (int i=0; i<canali.size(); i++) {
        canali[i].Qj = get_Q(i,x);
        canali[i].hj = get_h(i,x);
    }

    return info;
}

//void Simulazione::do_stuff(){/*
	//t_sim = (pthread_t) malloc(sizeof(pthread_t));
	//pthread_create(&t_sim, NULL, run, (void*) &t_sim);*/
	//t_sim = new std::thread;
	//*t_sim = std::thread(run);
//}

void Simulazione::run(){
	auto start = std::chrono::steady_clock::now();
	
	std::ofstream file;
    file.open("simulation.dat", std::ios::out);
	int i = 0;
	//for (int i = 0; i < 300; i++) {
	while(!finish){
			std::unique_lock<std::mutex> lk(m);
			cv.wait(lk, []{return ready;});
        //std::cout << "Step: " << i << " Time:" << i*canali[0].dt << " s" << std::endl;

        int info = step();
			lk.unlock();
    		cv.notify_all();
        std::cout << "Info:" << info << std::endl;

        file << canali.size() << " ";
        for (int m = 0; m < canali.size(); m++) {
            file << canali[m].N << " ";
        }
        for (int m = 0; m < canali.size(); m++) {
            for (int i = 0; i < canali[m].N; ++i) {
                file << canali[m].Qj[i] << " ";
            }
            for (int i = 0; i < canali[m].N; ++i) {
                file << canali[m].hj[i] << " ";
            }
        }
        file << std::endl;
		i++;
    }
    file.close();

    /* Controllo quanto tempo è passato */
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Execution Time:" << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
	//pthread_exit(NULL);
}

/*void Simulazione::setGlWindow(MyGlWindow *w){
	window = w;
}*/
