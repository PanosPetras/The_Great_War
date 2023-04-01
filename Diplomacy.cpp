#include "Diplomacy.h"

CountryPair::CountryPair(Country* C1, Country* C2) {
	if (C1 == nullptr || C2 == nullptr) {
		throw new _exception();
	}

	c1 = C1;
	c2 = C2;
}

bool CountryPair::operator==(const CountryPair& c) const {
	if (this->c1 != c.c1) {
		if (this->c1 == c.c2) {
			if (this->c2 == c.c1) {
				return true;
			}
		} 
	} else {
		if (this->c2 == c.c2) {
			return true;
		}
	}

	return false;
}

Country* CountryPair::GetC1() const {
	return c1;
}

Country* CountryPair::GetC2() const {
	return c2;
}

Relations::Relations(int relations, bool allied) {
	relationsValue = relations;
	this->allied = allied;
}

void Relations::ImproveRelations() {
	if (relationsValue < RELATIONS_LIMIT) {
		relationsValue += 15;
	}

	if (relationsValue > RELATIONS_LIMIT) {
		relationsValue = RELATIONS_LIMIT;
	}
}

void Relations::WorsenRelations() {
	if (relationsValue > -RELATIONS_LIMIT) {
		relationsValue -= 15;
	}

	if (relationsValue < -RELATIONS_LIMIT) {
		relationsValue = -RELATIONS_LIMIT;
	}
}

int Relations::GetRelationsValue() const {
	return relationsValue;
}

void Relations::CreateAlliance() {
	allied = true;
}

void Relations::BreakAlliance() {
	allied = false;
}

bool Relations::GetIfAllied() const {
	return allied;
}

Diplomacy::Diplomacy() {
	relations = new std::unordered_map<CountryPair, Relations>();
}

Diplomacy::~Diplomacy() {
	delete relations;
}
