#pragma once
#include "stdafx.h"
#include "ElemGraph.h"
#include "Graph.h"
#include "Pompa.h"
#include "Canale.h"
#include "Nodo.h"

class MyGlWindow : public Fl_Gl_Window{
	void draw();
	int handle(int);

	private:
		Graph *map;
		GLuint tex_pump, tex_terrain;
		float zoom_factor, dx, dy, main_coord_x, main_coord_y, main_coord_z, click_x, click_y;
		GLuint loadTexture(std::string filename, int width, int height);
		void drawLine(float x1, float y1, float x2, float y2, int samples, Nodo*, Nodo*);
		void drawMap();
		void drawDFS(int u);
		void drawFilledCircle(float x1, float y1, double radius);
		void drawPump(float x, float y);
		void drawBackground();
		void drawGraph(ElemGraph *v);

	public:
		MyGlWindow(int X, int Y, int W, int H, const char *L);
		~MyGlWindow();
		void setMap(Graph *g);

};

