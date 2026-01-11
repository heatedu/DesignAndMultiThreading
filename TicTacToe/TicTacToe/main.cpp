#include "TicTacToeGame.h"
#include "HumanPlayerStrategy.h"

int main() {
    PlayerStrategy* s1 = new HumanPlayerStrategy("Player X");
    PlayerStrategy* s2 = new HumanPlayerStrategy("Player O");

    TicTacToeGame game(s1, s2);
    game.play();

    delete s1;
    delete s2;

    return 0;
}
