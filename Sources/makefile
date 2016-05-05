CC  = $(shell fltk-config --cc)
CXX = $(shell fltk-config --cxx)

CFLAGS   = $(shell fltk-config --cflags) -Wall -O3 -I/usr/local/include -L/usr/local -lpng12 -lcminpack -lz -lm -lGLEW -lglut -lGLU -lGL -lX11 -lpthread -std=c++11
CXXFLAGS = $(shell fltk-config --cxxflags) -Wall -O3 -I/usr/local/include -L/usr/local -lpng12 -lcminpack -lz -lm -lGLEW -lglut -lGLU -lGL -lX11 -lpthread -std=c++11

LINKFLTK = $(shell fltk-config --ldstaticflags)
LINKFLTK_GL = $(shell fltk-config --use-gl --ldstaticflags)
LINKFLTK_IMG = $(shell fltk-config --use-images --ldstaticflags)

STRIP     = strip
POSTBUILD = fltk-config --post #required on OSX

all: riversim

riversim: main.o ElemGraph.o MyGlWindow.o Elemento.o Nodo.o Canale.o stdafx.o WindowManager.o ProvaParser.o Generatore.o Graph.o Simulazione.o
	$(CXX) -o $@ main.o ElemGraph.o MyGlWindow.o Elemento.o Nodo.o Canale.o stdafx.o WindowManager.o ProvaParser.o Generatore.o Graph.o Simulazione.o $(LINKFLTK_GL) -lpng12 -lcminpack -lz -lglut
	$(STRIP) $@
	$(POSTBUILD) $@

main.o: main.cpp stdafx.h WindowManager.h ProvaParser.h Generatore.h
	$(CXX) -c $< $(CXXFLAGS)

stdafx.o: stdafx.cpp stdafx.h
	$(CXX) -c $< $(CXXFLAGS)

WindowManager.o: WindowManager.cpp WindowManager.h stdafx.h MyGlWindow.h Generatore.h 
	$(CXX) -c $< $(CXXFLAGS)

ProvaParser.o: ProvaParser.cpp ProvaParser.h stdafx.h Parser.h ElemGraph.h Elemento.h Nodo.h Canale.h 
	$(CXX) -c $< $(CXXFLAGS)

Generatore.o: Generatore.cpp Generatore.h Parser.h Graph.h stdafx.h
	$(CXX) -c $< $(CXXFLAGS)

MyGlWindow.o: MyGlWindow.cpp MyGlWindow.h stdafx.h ElemGraph.h Graph.h Canale.h Nodo.h
	$(CXX) -c $< $(CXXFLAGS)

Graph.o: Graph.cpp Graph.h stdafx.h ElemGraph.h
	$(CXX) -c $< $(CXXFLAGS)

ElemGraph.o: ElemGraph.cpp ElemGraph.h stdafx.h
	$(CXX) -c $< $(CXXFLAGS)

Canale.o: Canale.cpp Canale.h stdafx.h Elemento.h
	$(CXX) -c $< $(CXXFLAGS)

Pompa.o: Pompa.cpp Pompa.h stdafx.h Elemento.h
	$(CXX) -c $< $(CXXFLAGS)

Elemento.o: Elemento.cpp Elemento.h stdafx.h
	$(CXX) -c $< $(CXXFLAGS)
	
Simulazione.o: Simulazione.cpp Simulazione.h stdafx.h Nodo.h Canale.h
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm *.o *.cpp~ *.h~ *.txt~ riversim
