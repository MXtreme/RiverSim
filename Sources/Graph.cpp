#include "Graph.h"

Graph::Graph(){
	dim = 0;
}

int Graph::getDim(){
	return dim;
}

bool Graph::isVisitato(int id){
	return visitato[id];
}

void Graph::setFalseVisitato(){
	for (size_t i = 0; i < visitato.size(); i++){
		visitato[i] = false;
	}
}

void Graph::setVisitato(int id){
	visitato[id] = true;
}

bool Graph::isEmpty(){
	if (dim == 0){
		return true;
	}
	else return false;
}

std::vector<Nodo*> Graph::DFS(ElemGraph * eg){
	std::vector<Canale*> next=eg->getNext();
	std::vector<Nodo*> adj_node;
	
	for(size_t i=0;i<next.size();i++){
		std::vector<std::string> adj=next[i]->adj;
		for(size_t k=0;k<node_list.size();k++){
			for(size_t j=0;j<adj.size();j++){
				if(node_list[k]->getElemento().name==adj[j] && node_list[k]->getElemento().name!=eg->getElemento().name){
					adj_node.push_back(node_list[k]->getElementPointer());
				}
			}
		}
	}
	
	return adj_node;
}

void Graph::addNode(ElemGraph* eg){
	node_list.push_back(eg);
}

void Graph::incrDim(){
	dim++;
}

ElemGraph* Graph::getNodeList(int id){
	return node_list[id];
}

void Graph::setNextNodeList(ElemGraph *node){
	node_list.push_back(node);
}

std::vector<ElemGraph*> Graph::getAllNodeList(){
	return node_list;
}

std::vector<Canale*> Graph::getAdj(ElemGraph* eg){
	return eg->getNext();
}

double Graph::getCanaleHeight(Nodo *a, Nodo *b){
	int canale=findCanale(a, b);
	double max_height=0;
	
	Canale tmp=sim->canali[canale];
	
	for(size_t i=0;i<tmp.hj.size();i++){
		if(tmp.hj[i]>max_height){
			max_height=tmp.hj[i];
		}
	}

	return max_height;
}

double Graph::getMaxHeight() {
	double max = 0;

	for (size_t i=0; i < sim->canali.size(); i++) {
		for (size_t j=0; j < sim->canali[i].hj.size(); j++) {
			if (sim->canali[i].hj[j] > max) max = sim->canali[i].hj[j];
		}
	}
	return max;
}

double Graph::getMaxFlow() {
	double max = 0;

	for (size_t i=0; i < sim->canali.size(); i++) {
		for (size_t j=0; j < sim->canali[i].hj.size(); j++) {
			if (sim->canali[i].hj[j] > max) max = sim->canali[i].hj[j];
		}
	}
	return max;
}


double Graph::getCanaleHeightPosition(Nodo *a, Nodo *b){
	int canale=findCanale(a, b);
	double max_height=getCanaleHeight(a, b);
	double height_peak=0;
	int segment=0;

	Canale tmp=sim->canali[canale];

	for(size_t i=0;i<tmp.hj.size();i++){
		if(tmp.hj[i]==max_height) segment=i;
	}

	height_peak=(100*segment)/tmp.hj.size();

	return height_peak;
}

double Graph::getCanaleFlow(Nodo *a, Nodo *b){
	int canale=findCanale(a, b);
	double max_flow=0;

	Canale tmp=sim->canali[canale];

	for(size_t i=0;i<tmp.Qj.size();i++){
		if(tmp.Qj[i]>max_flow) max_flow=tmp.Qj[i];
	}

	return max_flow;
}

double Graph::getCanaleFlowPosition(Nodo *a, Nodo *b){
	int canale=findCanale(a, b);
	double max_flow=getCanaleFlow(a, b);
	double flow_peak=0;
	int segment=0;

	Canale tmp=sim->canali[canale];

	for(size_t i=0;i<tmp.Qj.size();i++){
		if(tmp.Qj[i]==max_flow) segment=i;
	}

	flow_peak=(100*segment)/tmp.Qj.size();

	return flow_peak;
}

int Graph::findCanale(Nodo *a, Nodo *b){
	int canale=-1;

	for(size_t i=0;i<a->canali_in.size();i++){
		for(size_t k=0;k<b->canali_out.size();k++){
			if(a->canali_in[i]==b->canali_out[k]){
				canale=a->canali_in[i];
			}
		}
	}

	if(canale==-1){
		for(size_t i=0;i<a->canali_out.size();i++){
			for(size_t k=0;k<b->canali_in.size();k++){
				if(a->canali_out[i]==b->canali_in[k]){
					canale=a->canali_out[i];
				}
			}
		}	
	}
	return canale;
}

int Graph::getNSections(Nodo* a, Nodo *b){
	int canale=findCanale(a, b);
	Canale tmp=sim->canali[canale];
	return tmp.N;
}

double Graph::getCanaleHeightSection(Nodo *a, Nodo *b, int sect){
	int canale=findCanale(a, b);
	Canale tmp=sim->canali[canale];
	return tmp.hj[sect];
}
double Graph::getCanaleFlowSection(Nodo *a, Nodo *b, int sect){
	int canale=findCanale(a, b);
	Canale tmp=sim->canali[canale];
	return tmp.Qj[sect];
}
