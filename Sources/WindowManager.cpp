#include "WindowManager.h"

extern char *file_name;
extern Generatore *gen;
extern Simulazione *sim;
extern std::mutex lock;
extern bool ready;
extern std::mutex m;
extern std::condition_variable cv;

WindowManager::WindowManager(int W, int H, const char *L)
: Fl_Window(W, H, L){

	// Variables initialization
	window = new MyGlWindow(0, 30, w(), h()-30, L);
	window->setMap(gen->getMap());

	menu_items[0] = { "&File", 0, 0, 0, FL_SUBMENU };
	menu_items[1] = { "&Open File...", FL_COMMAND + 'o', (Fl_Callback *)open_cb };
	menu_items[2] = { "&Exit", FL_COMMAND + 'q', (Fl_Callback *)quit_cb, 0 };
	menu_items[3] = { 0 };

	menu_items[4] = { "&Simulator", 0, 0, 0, FL_SUBMENU };
	menu_items[5] = { "&Run", FL_COMMAND + 'r', (Fl_Callback *)run_cb };
	menu_items[6] = { "&Stop", FL_COMMAND + 's', (Fl_Callback *)stop_cb };
	menu_items[7] = { 0 };

	menu_items[8] = { "&Help", 0, 0, 0, FL_SUBMENU };
	menu_items[9] = { "&Info", FL_COMMAND + 'i', (Fl_Callback *)info_cb };
	menu_items[10] = { 0 };

	menu_items[11] = { 0 };

	// Window building
	menu_bar = new Fl_Menu_Bar(0, 0, W, 30);
	menu_bar->copy(menu_items);
	Fl::add_timeout(1.0/60.0, Timer_CB, (void*)window);
	//window->redraw();
	end();
	//idle();
}

MyGlWindow* WindowManager::getOpenGLWindow(){
	return window;
}
/*
void WindowManager::idle(){
	std::chrono::milliseconds inizio = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

while(true){
	std::chrono::milliseconds now = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(now - inizio).count();
		if(time>=1){
			inizio = now;
			window->redraw();	
		}	else{std::this_thread::sleep_for(std::chrono::milliseconds(1000));}
}
}
*/
void open_cb(Fl_Widget*, void*){
	file_name = fl_file_chooser("Open File", "*.txt", file_name);
	gen->build(file_name);
	sim->canali=gen->getCanali();
	sim->nodi=gen->getPompe();
	sim->init();
	printf("%f\n", sim->canali[0].hj[0]);
	sim->start();
	printf("%s loaded\n", file_name);	//DEBUG
}

void run_cb(Fl_Widget*, void*){
	if(gen->getBuilt()==true){
		std::lock_guard<std::mutex> lk(m);
      ready = true;
		cv.notify_all();
	}
}

void stop_cb(Fl_Widget*, void*){
	std::lock_guard<std::mutex> lk(m);
	ready = false;
	cv.notify_all();
}

void quit_cb(Fl_Widget*, void*){
		sim->finish = true;
		std::lock_guard<std::mutex> lk(m);
      ready = true;
		sim->finish=true;
		cv.notify_all();
		printf("joining...\n");
		delete sim;
		//sim->t_sim->detach();
		printf("done!\n");
		exit(0);
}

void info_cb(Fl_Widget*, void*){
	exit(0);
}
