#include "GameStates.h"
#include "GameContext.h"
#include "Player.h"

// XTurnState implementation
void XTurnState::next(GameContext& context, Player* player, bool hasWon) {
    if (hasWon) {
        context.setState(player->getSymbol() == Symbol::X ? 
            (GameState*)new XWonState() : (GameState*)new OWonState());
    } else {
        // Switch to OTurnState
        context.setState(new OTurnState());
    }
}

// OTurnState implementation
void OTurnState::next(GameContext& context, Player* player, bool hasWon) {
    if (hasWon) {
        context.setState(player->getSymbol() == Symbol::X ? 
            (GameState*)new XWonState() : (GameState*)new OWonState());
    } else {
        // Switch to XTurnState
        context.setState(new XTurnState());
    }
}

