#include "stdafx.h"
#include "WindowManager.h"
#include "ProvaParser.h"
#include "Generatore.h"
#include "Simulazione.h"

char *file_name;
Generatore *gen;
Simulazione *sim;
std::condition_variable cv;
std::mutex lock;
bool ready = false;
std::mutex m;

int main(int argc, char* argv[]){
	ProvaParser p;
	gen = new Generatore(&p);
	sim = new Simulazione();

	WindowManager win(1024, 600, "RiverSim");
	win.show(argc, argv);
    
	return (Fl::run());
}
