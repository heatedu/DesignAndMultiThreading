#pragma once
#include <iostream>
#include "Board.h"
#include "Player.h"
#include "GameContext.h"
#include "GameStates.h"

class TicTacToeGame {
    Board board;
    Player playerX;
    Player playerO;
    Player* currentPlayer;
    GameContext* context;

public:
    TicTacToeGame(PlayerStrategy* xStrat, PlayerStrategy* oStrat)
        : board(3, 3),
        playerX(Symbol::X, xStrat),
        playerO(Symbol::O, oStrat),
        currentPlayer(&playerX) {
        context = new GameContext();
    }

    ~TicTacToeGame() {
        delete context;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == &playerX) ? &playerO : &playerX;
    }

    void announceResult(GameState* s) {
        if (dynamic_cast<XWonState*>(s)) std::cout << "Player X wins!\n";
        else if (dynamic_cast<OWonState*>(s)) std::cout << "Player O wins!\n";
        else std::cout << "It's a draw!\n";
    }

    void play();
};
