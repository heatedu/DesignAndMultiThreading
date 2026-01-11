#include "TicTacToeGame.h"

void TicTacToeGame::play() {
    while (!context->isGameOver()) {
        board.printBoard();

        Position move = currentPlayer->getStrategy()->makeMove(board); //move chalo
        board.makeMove(move, currentPlayer->getSymbol());

        board.checkAndUpdateGameStateAfterEachMove(*context, currentPlayer); //state update kro

        if (!context->isGameOver()) { //agar jeete toh kuch nhi vrna player switch kro
            switchPlayer();
        }
    }
    board.printBoard();
    announceResult(context->getState());
}
