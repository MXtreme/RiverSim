#pragma once
/*
#ifndef MESA
glDrawBuffer(GL_FRONT_AND_BACK);
#endif
*/
#include "stdafx.h"
#include "MyGlWindow.h"
#include "Generatore.h"
#include "Simulazione.h"


void open_cb(Fl_Widget*, void*);
void run_cb(Fl_Widget*, void*);
void stop_cb(Fl_Widget*, void*);
void quit_cb(Fl_Widget*, void*);
void info_cb(Fl_Widget*, void*);

class WindowManager : public Fl_Window{

	static void Timer_CB(void *userdata) {
        MyGlWindow *pb = (MyGlWindow*)userdata;
        pb->redraw();
        Fl::repeat_timeout(1.0/1, Timer_CB, userdata);
    }

	private:
		MyGlWindow *window;
		Fl_Menu_Bar *menu_bar;
		Fl_Menu_Item menu_items[12];
		//void idle();

	public:
		WindowManager(int W, int H, const char *L);
		MyGlWindow* getOpenGLWindow();
};
