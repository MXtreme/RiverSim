#include "stdafx.h"
#include "ProvaParser.h"

void ProvaParser::readFile(std::vector<Canale> *can, std::vector<Nodo> *pom, std::string name_file){
	std::ifstream infile(name_file);

	while (!infile.eof()){
		std::getline(infile, line);
		if (line.find("nome:") != std::string::npos){
			nome = line.substr(5, line.size());
		}
		else if (line.find("tipo:") != std::string::npos){
			if (line.substr(5, line.size()) == "c"){
				tipo = 0;
			}
			//else if (line.substr(5, line.size()) == "p"){
			else if (line.substr(5, 1) == "p"){
				tipo = 1;
				if (line.substr(6, 1) == "h") {
					tipo_pompa = Nodo::fixed_h;
				}
				else if (line.substr(6, 1) == "q") {
					tipo_pompa = Nodo::fixed_Q;
				}
				else if (line.substr(6, 1) == "j") {
					tipo_pompa = Nodo::junction;
				}
			}
		}
		else if (line.find("lunghezza:") != std::string::npos){
			lunghezza = stof(line.substr(10, line.size()));
		}
		else if (line.find("larghezza:") != std::string::npos){
			larghezza = stof(line.substr(10, line.size()));
		}
		else if (line.find("pendenza:") != std::string::npos){
			pendenza = stof(line.substr(9, line.size()));
		}
		else if (line.find("portata:") != std::string::npos){
			portata = stof(line.substr(8, line.size()));
		}
		else if (line.find("altezza:") != std::string::npos){
			altezza = stof(line.substr(8, line.size()));
		}
		else if (line.find("adiac:") != std::string::npos){
			if (line.find("null") == std::string::npos){
				tmp = line.substr(6, line.size());
				offset = 0;
				count = 0;
				next = estraiAdiacenti(tmp);
			}
		}
		else if (line.find("x:") != std::string::npos){
			x = stof(line.substr(2, line.size()));
		}
		else if (line.find("y:") != std::string::npos){
			y = stof(line.substr(2, line.size()));
		}
		else if (line.find("end") != std::string::npos){
			if (tipo == 0){
				Canale *c = new Canale(nome, lunghezza, larghezza, pendenza, next);
				c->Qj.assign((unsigned long) c->N, portata);
    			c->hj.assign((unsigned long) c->N, altezza);
				can->push_back(*c);
			}
			else if (tipo == 1){
				Nodo *p = new Nodo(nome, portata, altezza, x, y, next, tipo_pompa);
    			p->h_set = altezza;
				p->Q_set = portata;
				pom->push_back(*p);
			}
		}
	}

	infile.close();
}

std::vector<std::string> ProvaParser::estraiAdiacenti(std::string tmp){
	std::vector<std::string> adiacenti;
	
	for (size_t i = 0; i < tmp.size(); i++){
		if (tmp.at(i) != ','){
			count++;
			offset++;
		}
		else{
			temp = (char*)malloc(sizeof(char)*count + 1);

			for (int j = 0; j < count; j++){
				temp[j] = tmp.at(j + (offset - count));
			}

			temp[count] = '\0';
			count = 0;
			offset++;
			adiacenti.push_back(temp);
			free(temp);
		}
	}
	temp = (char*)malloc(sizeof(char)*count + 1);

	for (int j = 0; j < count; j++){
		temp[j] = tmp.at(j + (offset - count));
	}

	temp[count] = '\0';
	adiacenti.push_back(temp);

	return adiacenti;
}
