#include "Diplomacy.h"

#include <stdexcept>

CountryPair::CountryPair(Country* C1, Country* C2) {
	if (C1 == nullptr || C2 == nullptr) {
		throw std::runtime_error("CountryPair got a nullptr");
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

Relations::Relations() {
	relationsValue = 0;
	allied = false;
}

Relations::Relations(int relations, bool allied) {
	relationsValue = relations;
	this->allied = allied;
}

void Relations::ImproveRelations(int value) {
	if (relationsValue < RELATIONS_LIMIT) {
		relationsValue += value;
	}

	if (relationsValue > RELATIONS_LIMIT) {
		relationsValue = RELATIONS_LIMIT;
	}
}

void Relations::WorsenRelations(int value) {
	if (relationsValue > -RELATIONS_LIMIT) {
		relationsValue -= value;
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
	ImproveRelations(80);
}

void Relations::BreakAlliance() {
	allied = false; 
	WorsenRelations(100);
}

bool Relations::GetIfAllied() const {
	return allied;
}

void Relations::ImposeEmbargo(std::string Instigator) {
	embargoes.insert(Instigator);
	WorsenRelations(100);
}

void Relations::LiftEmbargo(std::string Instigator) {
	embargoes.erase(Instigator);
	ImproveRelations(20);
}

bool Relations::GetIfEmbargoExists() {
	return embargoes.size() != 0;
}

bool Relations::GetIfHasEmbargo(std::string Instigator) {
	if (!GetIfEmbargoExists()) {
		return false;
	}

	return embargoes.find(Instigator) != embargoes.end();
}

Diplomacy::Diplomacy() {
	relations = new std::unordered_map<CountryPair, Relations>();
}

Diplomacy::~Diplomacy() {
	delete relations;
}

Request::Request(RequestType id, int senderIndex, std::string senderTag, Relations& relations) {
	this->id = id;
	index = senderIndex;
	tag = senderTag;
	rel = &relations;
}

void Request::Accept() {
	switch (id) {
		case alliance:
			rel->CreateAlliance();
			break;
		case tradeDeal:
			break;
		case peaceTreaty:
			break;
		default:
			break;
	}
}

void Request::Decline() {
	switch (id) {
		case alliance:
			rel->WorsenRelations();
			break;
		case tradeDeal:
			break;
		case peaceTreaty:
			break;
		default:
			break;
	}
}

Relations Request::GetRelations() {
	return *rel;
}

std::string Request::GetSender() {
	return tag;
}

int Request::GetSenderIndex() {
	return index;
}
