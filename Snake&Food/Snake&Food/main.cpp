// snake.cpp
#include <iostream>
#include<vector>
#include<deque>
#include<unordered_map>
#include<unordered_set>
#include <string>
using namespace std;

// ==============================
// Pair equivalent
// ==============================
template<typename T, typename U>
using Pair = pair<T, U>;

// Hash for pair<int,int>
struct PairHash {
    size_t operator()(const Pair<int, int>& p) const {
        return (static_cast<size_t>(p.first) << 32) ^ p.second;
    }
};

// Equality for pair<int,int>
struct PairEqual {
    bool operator()(const Pair<int, int>& a, const Pair<int, int>& b) const {
        return a.first == b.first && a.second == b.second;
    }
};


// ==============================
// MovementStrategy interface
// ==============================
class MovementStrategy {
public:
    virtual Pair<int, int> getNextPosition(Pair<int, int> currentHead, string direction) = 0;
    virtual ~MovementStrategy() = default;
};

class HumanMovementStrategy : public MovementStrategy {
public:
    Pair<int, int> getNextPosition(Pair<int, int> currentHead, string direction) override {
        int row = currentHead.first;
        int col = currentHead.second;

        if (direction == "U") return { row - 1, col };
        if (direction == "D") return { row + 1, col };
        if (direction == "L") return { row, col - 1 };
        if (direction == "R") return { row, col + 1 };

        return currentHead;
    }
};

// AI movement strategy placeholder
class AIMovementStrategy : public MovementStrategy {
public:
    Pair<int, int> getNextPosition(Pair<int, int> currentHead, string direction) override {
        // Placeholder - actual pathfinding logic would go here
        return currentHead;
    }
};

// ==============================
// FoodItem hierarchy
// ==============================
class FoodItem {
protected:
    int row, column;
    int points;

public:
    FoodItem(int r, int c) : row(r), column(c), points(0) {}

    int getRow() const { return row; }
    int getColumn() const { return column; }
    int getPoints() const { return points; }

    virtual ~FoodItem() = default;
};

class NormalFood : public FoodItem {
public:
    NormalFood(int r, int c) : FoodItem(r, c) { points = 1; }
};

class BonusFood : public FoodItem {
public:
    BonusFood(int r, int c) : FoodItem(r, c) { points = 3; }
};

// ==============================
// FoodFactory(Not used can be used when asked by interviewer)
// ==============================
class FoodFactory {
public:
    static FoodItem* createFood(int position[2], string type) {
        if (type == "bonus") {
            return new BonusFood(position[0], position[1]);
        }
        return new NormalFood(position[0], position[1]);
    }
};

// ==============================
// GameBoard (Singleton)
// ==============================
class GameBoard {
private:
    static GameBoard* instance;
    int width, height;

    GameBoard(int w, int h) : width(w), height(h) {}

public:
    static GameBoard* getInstance(int w, int h) {
        if (instance == nullptr) {
            instance = new GameBoard(w, h);
        }
        return instance;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

GameBoard* GameBoard::instance = nullptr;

// ==============================
// SnakeGame class
// ==============================
class SnakeGame {
public:
    GameBoard* board;
    deque<Pair<int, int>> snake;
    unordered_map<Pair<int, int>, bool, PairHash, PairEqual> snakeMap;
    unordered_set<Pair<int, int>, PairHash> foodSet;
    MovementStrategy* movementStrategy;

    SnakeGame(int width, int height, const vector<vector<int>>& foodArr) {
        this->board = GameBoard::getInstance(width, height);

        for (auto& f : foodArr) {
            foodSet.insert({f[0], f[1]});
        }

        Pair<int, int> initialPos = { 0, 0 };
        snake.push_front(initialPos);
        snakeMap[initialPos] = true;

        movementStrategy = new HumanMovementStrategy();
    }

    void setMovementStrategy(MovementStrategy* strategy) {
        movementStrategy = strategy;
    }

    int move(string direction) {
        Pair<int, int> currentHead = snake.front();
        Pair<int, int> newHead = movementStrategy->getNextPosition(currentHead, direction);

        int newHeadRow = newHead.first;
        int newHeadCol = newHead.second;

        // 1. Check boundary collision first
        bool crossesBoundary =
            newHeadRow < 0 || newHeadRow >= board->getHeight() ||
            newHeadCol < 0 || newHeadCol >= board->getWidth();

        if (crossesBoundary) {
            return -1;  // Wall collision
        }

        // 2. Check if ate food
        bool ateFood = foodSet.count(newHead) > 0;

        /********IMPORTANT*****/
      /*Here only we are pushing newHead in front and only popping from back
      Popping from front and pushing at back is not needed
      in this way even if snake will not eat food we will update head and back of dequeue
      will always point to snake tail*/

        // 3. Remove tail BEFORE checking self-collision (if didn't eat food)
        Pair<int, int> currentTail = snake.back();
        if (!ateFood) {
            snakeMap.erase(currentTail);
            snake.pop_back();
        }
        else {
            foodSet.erase(newHead);
        }

        // 4. Now check self-collision (tail already removed if needed)
        bool bitesItself = snakeMap.count(newHead) > 0;

        if (bitesItself) {
            return -2;  // Self collision
        }

        // 5. Add new head
        snake.push_front(newHead);
        snakeMap[newHead] = true;

        return (int)snake.size() - 1;// returns score and score = number of foods eaten, not snake length:
    }
};

// ==============================
// displayGameState (simple)
// ==============================
void displayGameState(SnakeGame& game) {
    cout << "\nCurrent snake length: " << game.snake.size() << "\n";
}

// ==============================
// Input converter
// ==============================
string convertInput(char c) {
    c = toupper(c);
    if (c == 'W') return "U";
    if (c == 'S') return "D";
    if (c == 'A') return "L";
    if (c == 'D') return "R";
    return "";
}

// ==============================
// Main driver (SnakeGameMain)
// ==============================
int main() {
    int width = 20;
    int height = 15;

    vector<vector<int>> foodPositions = {
        {5, 5},
        {10, 8},
        {3, 12},
        {8, 17},
        {12, 3}
    };

    SnakeGame game(width, height, foodPositions);

    cout << "===== SNAKE GAME =====\n";
    cout << "Controls: W (Up), S (Down), A (Left), D (Right), Q (Quit)\n";

    bool gameRunning = true;
    int score = 0;

    while (gameRunning) {
        displayGameState(game);

        cout << "Enter move (W/A/S/D) or Q to quit: ";
        string input;
        cin>>input;

        if (input.empty()) continue;

        char ch = toupper(input[0]);
        if (ch == 'Q') {
            cout << "Game ended by player. Final score: " << score << "\n";
            break;
        }

        string direction = convertInput(ch);
        if (direction == "") {
            cout << "Invalid input!\n";
            continue;
        }

        score = game.move(direction);

        if (score == -1) {
            cout << "GAME OVER! Snake hit the wall!\n";
            cout << "Final score: " << (game.snake.size() - 1) << "\n";
            gameRunning = false;
        }
        else if (score == -2) {
            cout << "GAME OVER! Snake collided with itself!\n";
            cout << "Final score: " << (game.snake.size() - 1) << "\n";
            gameRunning = false;
        }
        else {
            cout << "Score: " << score << "\n";
        }
    }

    cout << "Thanks for playing!\n";
    return 0;
}
