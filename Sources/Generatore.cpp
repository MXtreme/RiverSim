#include "stdafx.h"
#include "Generatore.h"

Generatore::Generatore(Parser *par){
	p = par;	
}

void Generatore::build(std::string name_file){
	p->readFile(&canali, &pompe, name_file);
	//printf("%d\n", canali.size());

	for (size_t i=0; i<pompe.size(); i++){
		for (size_t k=0; k<canali.size(); k++){
			for(size_t j=0; j<canali[k].adj.size();j++){
				if(pompe[i].name==canali[k].adj[j]){
					if(j==0){ pompe[i].canali_out.push_back(k);/* printf("pompe[%d].canali_out.push_back(%d)\n", i, k); */}
					else { pompe[i].canali_in.push_back(k);/* printf("pompe[%d].canali_in.push_back(%d)\n", i, k); */}
				}
			}
		}
		pompe_node.push_back(new ElemGraph(&pompe[i], map.getDim()));
		map.addNode(pompe_node[pompe_cap]);
		pompe_cap++;
				
		map.incrDim();
	}
	
	for(size_t i=0; i<pompe_node.size(); i++){
		for(size_t k=0; k<pompe_node[i]->getElemento().canali_in.size(); k++){
			for(size_t j=0;j<canali[pompe_node[i]->getElemento().canali_in[k]].adj.size();j++){
				if(pompe_node[i]->getElemento().name==canali[pompe_node[i]->getElemento().canali_in[k]].adj[j]){
					//printf("%s -> %s\n", pompe_node[i]->getElemento().name.c_str(), canali[pompe_node[i]->getElemento().canali_in[k]].name.c_str());
					pompe_node[i]->setNext(&canali[pompe_node[i]->getElemento().canali_in[k]]);
				}
			}
		}
		
		for(size_t k=0; k<pompe_node[i]->getElemento().canali_out.size(); k++){
			for(size_t j=0;j<canali[pompe_node[i]->getElemento().canali_out[k]].adj.size();j++){
				if(pompe_node[i]->getElemento().name==canali[pompe_node[i]->getElemento().canali_out[k]].adj[j]){
					//printf("%s -> %s\n", pompe_node[i]->getElemento().name.c_str(), canali[pompe_node[i]->getElemento().canali_out[k]].name.c_str());
					pompe_node[i]->setNext(&canali[pompe_node[i]->getElemento().canali_out[k]]);
				}
			}
		}
	}
	
	built=true;
}

Graph* Generatore::getMap(){
	return &map;
}

std::vector<Canale> Generatore::getCanali(){
	return canali;
}

std::vector<Nodo> Generatore::getPompe(){
	return pompe;
}

bool Generatore::getBuilt(){
	return built;
}
