#include "Diplomacy.h"
#include "Country.h"

#include <algorithm>
#include <stdexcept>

CountryPair::CountryPair(Country* C1, Country* C2) : c1(C1), c2(C2) {
    if(C1 == nullptr || C2 == nullptr) {
        throw std::runtime_error("CountryPair got a nullptr");
    }
}

bool CountryPair::operator==(const CountryPair& c) const {
    if(this->c1 != c.c1) {
        if(this->c1 == c.c2) {
            if(this->c2 == c.c1) {
                return true;
            }
        }
    } else {
        if(this->c2 == c.c2) {
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

std::string CountryPair::toString() const {
    std::string t1 = c1 ? c1->GetName() : "";
    std::string t2 = c2 ? c2->GetName() : "";
    return t1 > t2 ? t1 + '\0' + t2 : t2 + '\0' + t1;
}

Relation::Relation() : Relation(0, false) {}

Relation::Relation(int relations, bool Allied) {
    relationsValue = relations;
    allied = Allied;
}

void Relation::ImproveRelations(int value) {
    relationsValue = std::min(relationsValue + value, RELATIONS_LIMIT);
}

void Relation::WorsenRelations(int value) {
    relationsValue = std::max(relationsValue - value, -RELATIONS_LIMIT);
}

int Relation::GetRelationsValue() const {
    return relationsValue;
}

void Relation::CreateAlliance() {
    allied = true;
    ImproveRelations(80);
}

void Relation::BreakAlliance() {
    allied = false;
    WorsenRelations(100);
}

bool Relation::GetIfAllied() const {
    return allied;
}

void Relation::ImposeEmbargo(std::string Instigator) {
    embargoes.push_back(Instigator);
    WorsenRelations(100);
}

void Relation::LiftEmbargo(std::string Instigator) {
    embargoes.erase(std::remove(embargoes.begin(), embargoes.end(), Instigator), embargoes.end());
    ImproveRelations(20);
}

bool Relation::GetIfEmbargoExists() const {
    return embargoes.size() != 0;
}

bool Relation::GetIfHasEmbargo(std::string Instigator) const {
    return std::find(embargoes.begin(), embargoes.end(), Instigator) != embargoes.end();
}

Request::Request(RequestType Id, unsigned senderIndex, std::string senderTag, Relation& relations) : id{Id}, index{senderIndex}, rel{relations}, tag(senderTag) {}

void Request::Accept() {
    switch(id) {
    case alliance:
        rel.CreateAlliance();
        break;
    case tradeDeal:
        break;
    case peaceTreaty:
        break;
    }
}

void Request::Decline() {
    switch(id) {
    case alliance:
        rel.WorsenRelations();
        break;
    case tradeDeal:
        break;
    case peaceTreaty:
        break;
    }
}

Relation& Request::GetRelations() const {
    return rel;
}

std::string Request::GetSender() const {
    return tag;
}

unsigned Request::GetSenderIndex() const {
    return index;
}

War::War(Country* aggressor, Country* defender) : factions({Faction(aggressor), Faction(defender)}) {}

bool War::GetIfTargetPairIsAtWar(const CountryPair& pair) const {
    if(factions[0].Contains(pair.GetC1())) {
        if(factions[1].Contains(pair.GetC2())) {
            return true;
        }
    } else {
        if(factions[1].Contains(pair.GetC1())) {
            if(factions[0].Contains(pair.GetC2())) {
                return true;
            }
        }
    }
    return false;
}

void War::JoinWar(Country* newParticipant, Country* onTheSideOf) {
    Faction& f = factions[1];

    if(factions[0].Contains(onTheSideOf)) {
        f = factions[0];
    }

    f.AddMember(newParticipant);
}

const std::set<Country*>& War::GetFaction(unsigned i) const {
    if(i != 0 && i != 1) {
        throw std::out_of_range("Faction index out of bounds");
    }
    return factions[i].GetFaction();
}

void War::AddClaim(Claim claim) {
    Faction& f = factions[1];

    if(factions[0].Contains(claim.GetOwner())) {
        f = factions[0];
    }

    f.AddClaim(claim);
}

const std::vector<Claim>& War::GetClaims(unsigned i) const {
    if(i != 0 && i != 1) {
        throw std::out_of_range("Faction index out of bounds");
    }
    return factions[i].GetClaims();
}

void War::AddScore(Country* instigator, int score) {
    Faction& f = factions[1];

    if(factions[0].Contains(instigator)) {
        f = factions[0];
    }

    f.AddScore(score);
}

int War::GetRelativeScore() const {
    return factions[0].GetScore() - factions[1].GetScore();
}

War::Faction::Faction(Country* c) {
    AddMember(c);
}

void War::Faction::AddMember(Country* c) {
    members.insert(c);
}

void War::Faction::RemoveMember(Country* c) {
    members.erase(c);
}

bool War::Faction::Contains(Country* c) const {
    return members.contains(c);
}

const std::set<Country*>& War::Faction::GetFaction() const {
    return members;
}

void War::Faction::AddScore(int c) {
    score += c;
}

int War::Faction::GetScore() const {
    return score;
}

void War::Faction::AddClaim(Claim c) {
    claims.push_back(c);
}

const std::vector<Claim>& War::Faction::GetClaims() const {
    return claims;
}

Claim::Claim(Country* Owner, Country* Target, ClaimType Type, void* Data) : owner(Owner), target(Target), type(Type), data(Data) {}

Country* Claim::GetOwner() const {
    return owner;
}

Country* Claim::GetTarget() const {
    return target;
}

ClaimType Claim::GetType() const {
    return type;
}
