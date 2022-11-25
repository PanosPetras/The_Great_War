#include "Diplomacy.h"
#include "Country.h"

Relations::Relations(){
}

Relations::Relations(Country* country1, Country* country2, int relations, bool allied){
	sides[0] = country1, sides[1] = country2;
	relationsValue = relations;
	this->allied = allied;
}

void Relations::ImproveRelations(){
	relationsValue += 15;
}

void Relations::WorsenRelations(){
	relationsValue -= 15;
}

int Relations::GetRelationsValue(){
	return relationsValue;
}

void Relations::CreateAlliance(){
	allied = true;
}

void Relations::BreakAlliance(){
	allied = false;
}

bool Relations::GetIfAllied(){
	return allied;
}

std::vector<std::string> Relations::toString(){
	std::vector<std::string> vec;

	vec.push_back(sides[0]->tag + "-" + sides[1]->tag);
	vec.push_back(sides[1]->tag + "-" + sides[0]->tag);

	return vec;
}

Diplomacy::Diplomacy(){
	relations = new std::unordered_map<std::string, Relations>();
}

Diplomacy::~Diplomacy(){
	delete relations;
}

Relations* Diplomacy::findRelation(std::vector<std::string> ids){
	if (relations->find(ids[0]) != relations->end()) {
		return &relations->at(ids[0]);
	}
	if (relations->find(ids[1]) != relations->end()) {
		return &relations->at(ids[1]);
	}
	return nullptr;
}
