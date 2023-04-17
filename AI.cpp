#include "AI.h"
#include "Diplomacy.h"

void AI::HandleRequest(Request& req) {
    if (req.GetRelations().GetRelationsValue() > 150) {
        req.Accept();
    }
    else {
        req.Decline();
    }
}
