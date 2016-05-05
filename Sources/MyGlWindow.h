#pragma once
#include "stdafx.h"
#include "ElemGraph.h"
#include "Graph.h"
#include "Pompa.h"
#include "Canale.h"
#include "Nodo.h"

#define VIEW_1 1
#define VIEW_2 2
class MyGlWindow : public Fl_Gl_Window{
	void draw();
	int handle(int);

	
	typedef enum {
   	MODE_BITMAP,
   	MODE_STROKE
	} mode_type;


	mode_type mode;
	int font_index;

	private:
		Graph *map;
		GLuint tex_pump, tex_terrain;
		float zoom_factor, dx, dy, main_coord_x, main_coord_y, main_coord_z, click_x, click_y, swipe_x, swipe_y, swipe_z;
		GLuint loadTexture(std::string filename, int width, int height);
		void drawLine(float x1, float y1, float x2, float y2, int samples, Nodo*, Nodo*);
		void drawMap();
		void drawDFS(int u);
		void drawFilledCircle(float x1, float y1, double radius);
		void drawPump(float x, float y);
		void drawOverlay();
		void drawBackground();
		void drawGraph(ElemGraph *v);
		void drawDFS(ElemGraph *prec, ElemGraph *pres);
		void drawString(char *string, float x, float y);
		void print_stroke_string(void* font, char* s);
		void print_bitmap_string(void* font, char* s);

	public:	
		MyGlWindow(int X, int Y, int W, int H, const char *L);
		~MyGlWindow();
		void setMap(Graph *g);
		int view;
};

