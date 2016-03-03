#ifndef GRAPH_H
#define GRAPH_H

#include "ElemGraph.h"
#include "stdafx.h"
#include "Simulazione.h"

extern Simulazione *sim;

class Graph{
protected:
	int dim;
	std::vector<bool> visitato;
	std::vector<ElemGraph*> node_list;

public:
	Graph();
	int getDim(); //ritorna la dimensione del grafo
	bool isVisitato(int); //ti dice se un nodo � stato visitato o meno
	void setFalseVisitato(); //setta tutti i nodi a non visitati
	void setVisitato(int); //setta un nodo come visitato
	bool isEmpty(); //restituisce true se il grafo � vuoto altrimenti false
	std::vector<Nodo*> DFS(ElemGraph*); //non � una DFS!! Fa tornare la lista dei nodi adiacenti a un nodo
	void addNode(ElemGraph*); //aggiunge un nodo al grafo
	bool findNodeByName(std::string, int); //NON SERVE
	int DFSByName(std::string); //mi dice se un nodo � presente o meno basandosi sul nome dell'elemento
	ElemGraph* getNode(ElemGraph*, int); //restituisce il nodo con l'id specificato come parametro
	ElemGraph* findNodeById(ElemGraph*, int); //DA FINIRE
	void incrDim(); //incremento di 1 la dimensione del grafo
	ElemGraph* getNodeList(int); //restituisce il puntatore al nodo corrispondente all'id
	void setNextNodeList(ElemGraph*); //imposto il puntatore all'elemento successivo
	std::vector<ElemGraph*> getAllNodeList(); //restituisce il vettore dei puntatori ai nodi
	std::vector<Canale*> getAdj(ElemGraph*);
	double getCanaleHeight(Nodo*, Nodo*);
	double getCanaleHeightPosition(Nodo*, Nodo*);
	double getCanaleFlow(Nodo*, Nodo*);
	double getCanaleFlowPosition(Nodo*, Nodo*);
	int findCanale(Nodo*, Nodo*);
	int getNSections(Nodo*, Nodo*);
	double getCanaleHeightSection(Nodo*, Nodo*, int);

	double getMaxHeight();

	double getCanaleFlux(Nodo *a, Nodo *b);

	double getMaxFlow();

	double getCanaleFlowSection(Nodo *a, Nodo *b, int sect);
};

#endif
