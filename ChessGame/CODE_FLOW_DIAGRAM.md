# Chess Game - Complete Code Flow Diagram

## Table of Contents
1. [Overall Architecture Flow](#overall-architecture-flow)
2. [Game Loop Flow](#game-loop-flow)
3. [Move Validation Flow](#move-validation-flow)
4. [Strategy Pattern - Piece Movement](#strategy-pattern---piece-movement)
5. [Factory Pattern - Piece Creation](#factory-pattern---piece-creation)
6. [Singleton Pattern - Board](#singleton-pattern---board)
7. [Game State Update Flow](#game-state-update-flow)
8. [Design Patterns Summary](#design-patterns-summary)

---

## Overall Architecture Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                         MAIN ENTRY POINT                         │
│                           main.cpp                               │
└────────────────────────────┬────────────────────────────────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │  Create Game    │
                    │ ("Alice","Bob") │
                    └────────┬────────┘
                             │
        ┌────────────────────┼────────────────────┐
        ▼                    ▼                    ▼
  ┌──────────┐      ┌──────────────┐      ┌──────────┐
  │ Player 1 │      │ Board::      │      │ Player 2 │
  │ (White)  │      │ getInstance()│      │ (Black)  │
  └──────────┘      │  SINGLETON   │      └──────────┘
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │ initialize() │
                    │   Board      │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
                    │ PieceFactory │
                    │   FACTORY    │
                    │ creates 32   │
                    │   pieces     │
                    └──────────────┘
```

**Key Components:**
- **main.cpp**: Entry point, creates game instance
- **Game**: Manages players, board, and game state
- **Board (Singleton)**: Single instance managing 8x8 grid
- **PieceFactory (Factory)**: Creates all 32 chess pieces

---

## Game Loop Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                         GAME LOOP                                │
│                  while (state == ACTIVE)                         │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
                      ┌───────────────┐
                      │ Display Board │
                      └───────┬───────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │ Get Current Player  │
                   │   & Turn Color      │
                   └──────────┬──────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │ Read User Input     │
                   │ (e.g., "e2 e4")     │
                   └──────────┬──────────┘
                              │
                   ┌──────────┴──────────┐
                   │                     │
                   ▼                     ▼
              ┌────────┐           ┌─────────┐
              │ "quit" │           │  Parse  │
              │        │           │Position │
              └───┬────┘           └────┬────┘
                  │                     │
                  ▼                     ▼
              ┌────────┐        ┌──────────────┐
              │  Exit  │        │  makeMove()  │
              └────────┘        └──────┬───────┘
                                       │
                                       ▼
                            ┌──────────────────┐
                            │   Move Valid?    │
                            └────┬────────┬────┘
                                 │        │
                            NO ◄─┘        └─► YES
                                 │             │
                                 ▼             ▼
                          ┌──────────┐  ┌─────────────┐
                          │ Display  │  │ switchTurn()│
                          │  Error   │  └──────┬──────┘
                          └─────┬────┘         │
                                │              ▼
                                │      ┌───────────────┐
                                │      │updateGameState│
                                │      └───────┬───────┘
                                │              │
                                └──────────────┴─────► Loop Again
```

**Flow Steps:**
1. Display current board state
2. Show whose turn it is
3. Get user input (move or quit)
4. Parse and validate move
5. Execute move if valid
6. Switch turns
7. Check for check/checkmate/stalemate
8. Repeat until game ends

---

## Move Validation Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                      makeMove(from, to)                          │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
                    ┌──────────────────┐
                    │ piece at "from"? │
                    └────┬────────┬────┘
                         │ NO     │ YES
                         ▼        ▼
                    ┌────────┐  ┌──────────────────┐
                    │ Error  │  │ Is it your turn? │
                    │ Return │  │ (color match?)   │
                    └────────┘  └────┬────────┬────┘
                                     │ NO     │ YES
                                     ▼        ▼
                                ┌────────┐  ┌──────────────────┐
                                │ Error  │  │ piece.canMove()  │
                                │ Return │  │   STRATEGY       │
                                └────────┘  └────┬────────┬────┘
                                                 │ NO     │ YES
                                                 ▼        ▼
                                            ┌────────┐  ┌──────────────┐
                                            │ Error  │  │board.movePiece│
                                            │ Return │  └──────┬───────┘
                                            └────────┘         │
                                                               ▼
                                                    ┌─────────────────┐
                                                    │ Check if own    │
                                                    │ king in check?  │
                                                    └────┬───────┬────┘
                                                         │ YES   │ NO
                                                         ▼       ▼
                                                    ┌────────┐ ┌────────┐
                                                    │ Undo   │ │Success │
                                                    │ Error  │ │ Return │
                                                    └────────┘ └────────┘
```

**Validation Checks (in order):**
1. **Piece exists** at source position?
2. **Correct color** (is it your turn)?
3. **Legal move** for this piece type (Strategy Pattern)?
4. **Path clear** for sliding pieces?
5. **Destination valid** (empty or enemy piece)?
6. **King not in check** after move?

---

## Strategy Pattern - Piece Movement

```
┌─────────────────────────────────────────────────────────────────┐
│                    piece.canMove(from, to)                       │
│                      STRATEGY PATTERN                            │
└─────────────────────────────┬───────────────────────────────────┘
                              │
              ┌───────────────┴───────────────┐
              │  movementStrategy->canMove()  │
              └───────────────┬───────────────┘
                              │
        ┌─────────────────────┼─────────────────────┐
        │                     │                     │
        ▼                     ▼                     ▼
┌──────────────┐      ┌──────────────┐     ┌──────────────┐
│KingMovement  │      │QueenMovement │     │RookMovement  │
│              │      │              │     │              │
│ • ±1 square  │      │ • Diagonal + │     │ • Horizontal │
│   any dir    │      │   Straight   │     │ • Vertical   │
│              │      │ • Unlimited  │     │ • Unlimited  │
│ • No jump    │      │ • Path clear │     │ • Path clear │
└──────────────┘      └──────────────┘     └──────────────┘
        │                     │                     │
        ▼                     ▼                     ▼
┌──────────────┐      ┌──────────────┐     ┌──────────────┐
│BishopMovement│      │KnightMovement│     │PawnMovement  │
│              │      │              │     │              │
│ • Diagonal   │      │ • L-shape    │     │ • Forward 1  │
│   only       │      │   (2+1 or    │     │ • Forward 2  │
│ • Unlimited  │      │    1+2)      │     │   (first)    │
│ • Path clear │      │ • CAN jump!  │     │ • Diag cap   │
└──────────────┘      └──────────────┘     └──────────────┘
        │                     │                     │
        └─────────────────────┴─────────────────────┘
                              │
                              ▼
                    ┌──────────────────┐
                    │ isPathClear()?   │
                    │ (for sliding     │
                    │  pieces only)    │
                    └──────┬───────────┘
                           │
                           ▼
                    ┌──────────────────┐
                    │ Check direction  │
                    │ Step by step     │
                    │ Any piece blocks?│
                    └──────┬───────────┘
                           │
                           ▼
                    ┌──────────────────┐
                    │ Destination OK?  │
                    │ • Empty, or      │
                    │ • Enemy piece    │
                    └──────┬───────────┘
                           │
                           ▼
                      ┌────────────┐
                      │Return true │
                      │  or false  │
                      └────────────┘
```

**Movement Rules by Piece:**

| Piece  | Pattern | Can Jump? | Distance | Special |
|--------|---------|-----------|----------|---------|
| King   | 8 directions | No | 1 square | - |
| Queen  | 8 directions | No | Unlimited | Rook + Bishop |
| Rook   | 4 directions (±row/col) | No | Unlimited | - |
| Bishop | 4 diagonals | No | Unlimited | - |
| Knight | L-shape (2+1) | **YES** | Fixed | Only jumper |
| Pawn   | Forward | No | 1-2 | Diagonal capture |

---

## Factory Pattern - Piece Creation

```
┌─────────────────────────────────────────────────────────────────┐
│              PieceFactory::createPiece(type, color)              │
│                        FACTORY PATTERN                           │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
                    ┌──────────────────┐
                    │  Switch on type  │
                    └────┬─────────────┘
                         │
        ┌────────┬───────┼───────┬────────┬─────────┐
        ▼        ▼       ▼       ▼        ▼         ▼
    ┌──────┐ ┌──────┐┌──────┐┌──────┐┌──────┐  ┌──────┐
    │ KING │ │QUEEN ││ ROOK ││BISHOP││KNIGHT│  │ PAWN │
    └──┬───┘ └──┬───┘└──┬───┘└──┬───┘└──┬───┘  └──┬───┘
       │        │       │       │       │         │
       └────────┴───────┴───────┴───────┴─────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │ Create concrete     │
              │ piece objects:      │
              │                     │
              │ new King(color)     │
              │ new Queen(color)    │
              │ new Rook(color)     │
              │ new Bishop(color)   │
              │ new Knight(color)   │
              │ new Pawn(color)     │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │ Each constructor    │
              │ assigns appropriate │
              │ MovementStrategy:   │
              │                     │
              │ King   → KingMov    │
              │ Queen  → QueenMov   │
              │ Rook   → RookMov    │
              │ Bishop → BishopMov  │
              │ Knight → KnightMov  │
              │ Pawn   → PawnMov    │
              └──────────┬──────────┘
                         │
                         ▼
              ┌─────────────────────┐
              │ Return unique_ptr   │
              │    <Piece>          │
              │                     │
              │ Polymorphic pointer │
              │ to base Piece class │
              └─────────────────────┘
```

**Factory Benefits:**
- ✅ Centralized creation logic
- ✅ Consistent initialization
- ✅ Easy to add new piece types
- ✅ Hides concrete class details
- ✅ Returns polymorphic base pointer

**Example Usage:**
```cpp
// In Board::initialize()
grid[0][0] = PieceFactory::createPiece(PieceType::ROOK, PieceColor::BLACK);
grid[0][1] = PieceFactory::createPiece(PieceType::KNIGHT, PieceColor::BLACK);
// ... creates all 32 pieces
```

---

## Singleton Pattern - Board

```
┌─────────────────────────────────────────────────────────────────┐
│                    Board::getInstance()                          │
│                     SINGLETON PATTERN                            │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
                    ┌──────────────────┐
                    │ Is static        │
                    │ instance NULL?   │
                    └────┬────────┬────┘
                         │ YES    │ NO
                         ▼        │
                  ┌──────────┐    │
                  │ Create   │    │
                  │ new Board│    │
                  │ instance │    │
                  └─────┬────┘    │
                        │         │
                        └─────┬───┘
                              │
                              ▼
                    ┌──────────────────┐
                    │ Return instance  │
                    │                  │
                    │ Same pointer for │
                    │ entire program!  │
                    └──────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│                      Singleton Guarantees                        │
└─────────────────────────────────────────────────────────────────┘

Private Constructor      →  Can't do: Board b;
Deleted Copy Ctor        →  Can't do: Board b2 = b1;
Deleted Assignment       →  Can't do: b1 = b2;
Static getInstance()     →  Only way: Board::getInstance();
Single static instance   →  Only ONE board exists globally
```

**Singleton Implementation:**
```cpp
class Board {
private:
    static Board* instance;           // Single instance
    Board();                          // Private constructor
    Board(const Board&) = delete;     // No copy
    Board& operator=(const Board&) = delete;  // No assignment

public:
    static Board* getInstance() {
        if (!instance) {
            instance = new Board();
        }
        return instance;
    }
    
    static void resetInstance() {     // For cleanup
        delete instance;
        instance = nullptr;
    }
};
```

**Why Singleton for Board?**
- ✅ Only one chess board exists in a game
- ✅ Global access from anywhere
- ✅ Prevents accidental duplication
- ✅ Controlled initialization

---

## Game State Update Flow

```
┌─────────────────────────────────────────────────────────────────┐
│                      updateGameState()                           │
│              Called after each successful move                   │
└─────────────────────────────┬───────────────────────────────────┘
                              │
                              ▼
                    ┌──────────────────┐
                    │ Is current       │
                    │ player's king    │
                    │ in check?        │
                    └────┬────────┬────┘
                         │ YES    │ NO
                         ▼        ▼
              ┌─────────────┐  ┌──────────────┐
              │ Does player │  │ Does player  │
              │ have valid  │  │ have valid   │
              │ moves?      │  │ moves?       │
              └──┬───────┬──┘  └──┬────────┬──┘
                 │ NO    │ YES    │ NO     │ YES
                 ▼       ▼        ▼        ▼
          ┌──────────┐ ┌─────┐ ┌────────┐ ┌──────┐
          │CHECKMATE!│ │CHECK│ │STALE-  │ │Active│
          │          │ │     │ │ MATE   │ │ Game │
          │Enemy Wins│ │     │ │ (Draw) │ │      │
          └─────┬────┘ └──┬──┘ └────┬───┘ └───┬──┘
                │         │         │         │
                ▼         │         ▼         │
        ┌───────────┐     │   ┌───────────┐  │
        │GameState: │     │   │GameState: │  │
        │           │     │   │STALEMATE  │  │
        │BLACK_WIN  │     │   │           │  │
        │or         │     │   │Draw - no  │  │
        │WHITE_WIN  │     │   │winner     │  │
        │           │     │   └───────────┘  │
        │Winner =   │     │                  │
        │opponent!  │     │                  │
        └───────────┘     │                  │
                │         │                  │
                └─────────┴──────────────────┘
                          │
                          ▼
                   ┌──────────────┐
                   │ Display msg  │
                   │ Return to    │
                   │ game loop    │
                   └──────────────┘
```

**Game State Outcomes:**

| King in Check? | Has Valid Moves? | Result | GameState | Winner |
|----------------|------------------|--------|-----------|--------|
| ✅ YES | ❌ NO | **CHECKMATE** | BLACK_WIN or WHITE_WIN | Opponent |
| ✅ YES | ✅ YES | **CHECK** | ACTIVE | - |
| ❌ NO | ❌ NO | **STALEMATE** | STALEMATE | Draw |
| ❌ NO | ✅ YES | **Normal** | ACTIVE | - |

**Code Logic:**
```cpp
void Game::updateGameState() {
    if (board->isInCheck(currentTurn)) {
        if (!board->hasValidMoves(currentTurn)) {
            // CHECKMATE - opponent wins!
            state = (currentTurn == WHITE) ? BLACK_WIN : WHITE_WIN;
            Player* winner = (currentTurn == WHITE) ? blackPlayer : whitePlayer;
            cout << "Checkmate! " << winner->getName() << " wins!\n";
        } else {
            // CHECK - king under attack but can escape
            cout << "Check!\n";
        }
    } else if (!board->hasValidMoves(currentTurn)) {
        // STALEMATE - no check but no legal moves
        state = STALEMATE;
        cout << "Stalemate!\n";
    }
    // else: normal game continues
}
```

---

## Design Patterns Summary

```
┌─────────────────────────────────────────────────────────────────┐
│                    4 DESIGN PATTERNS USED                        │
└─────────────────────────────────────────────────────────────────┘

╔════════════════════════════════════════════════════════════════╗
║ 1. STRATEGY PATTERN - Piece Movement Behaviors                 ║
╚════════════════════════════════════════════════════════════════╝

Purpose:  Encapsulate different movement algorithms
Pattern:  Each piece has a MovementStrategy object

         MovementStrategy (interface)
                 ▲
                 │ implements
       ┌─────────┼─────────┐
       │         │         │
  KingMovement QueenMovement ... PawnMovement

Benefits:
  ✓ Easy to add new piece types
  ✓ Movement logic isolated and testable
  ✓ Follows Open/Closed Principle
  ✓ Runtime strategy swapping possible

Usage:
  Piece has-a MovementStrategy
  piece->canMove() → delegates to strategy->canMove()


╔════════════════════════════════════════════════════════════════╗
║ 2. SINGLETON PATTERN - Board Instance                          ║
╚════════════════════════════════════════════════════════════════╝

Purpose:  Ensure only ONE board exists
Pattern:  Private constructor + static instance

         Board
           │
    ┌──────┴──────┐
    │ static      │
    │ instance*   │
    │             │
    │ getInstance()│ ← Only access method
    └─────────────┘

Benefits:
  ✓ Global access point
  ✓ Controlled instantiation
  ✓ Prevents multiple boards
  ✓ Lazy initialization

Usage:
  Board* board = Board::getInstance();
  // Always returns same instance


╔════════════════════════════════════════════════════════════════╗
║ 3. FACTORY PATTERN - Piece Creation                            ║
╚════════════════════════════════════════════════════════════════╝

Purpose:  Centralize object creation
Pattern:  Static factory method creates pieces

         PieceFactory
              │
         createPiece(type, color)
              │
       ┌──────┴──────┐
       │             │
    returns      returns
  unique_ptr    Piece*
    <Piece>   (polymorphic)

Benefits:
  ✓ Hides concrete classes
  ✓ Consistent initialization
  ✓ Easy to extend (add new pieces)
  ✓ Single point of creation logic

Usage:
  auto piece = PieceFactory::createPiece(KING, WHITE);
  // Returns properly initialized piece with strategy


╔════════════════════════════════════════════════════════════════╗
║ 4. ENUM STATE MANAGEMENT - Game States                         ║
╚════════════════════════════════════════════════════════════════╝

Purpose:  Type-safe game state tracking
Pattern:  Enum class for state machine

         GameState (enum class)
              │
       ┌──────┼──────┬──────┬──────┐
       │      │      │      │      │
    ACTIVE  SAVED  BLACK_WIN WHITE_WIN STALEMATE

Benefits:
  ✓ Type-safe (can't assign wrong values)
  ✓ Clear state representation
  ✓ Compiler-checked
  ✓ Self-documenting code

Usage:
  GameState state = GameState::ACTIVE;
  if (state == GameState::CHECKMATE) { ... }


╔════════════════════════════════════════════════════════════════╗
║                    Pattern Interactions                         ║
╚════════════════════════════════════════════════════════════════╝

  Game
    │
    ├─► Board (Singleton)
    │     │
    │     └─► 8x8 grid of Piece*
    │              │
    │              └─► Created by PieceFactory (Factory)
    │                        │
    │                        └─► Has MovementStrategy (Strategy)
    │
    └─► GameState (Enum)

Flow:
  1. Game uses Board::getInstance() (Singleton)
  2. Board calls PieceFactory::createPiece() (Factory)
  3. Each Piece gets MovementStrategy (Strategy)
  4. Game tracks state with GameState enum (Enum)
```

---

## Complete Move Execution Timeline

```
User inputs "e2 e4"
        │
        ▼
┌───────────────────────────────────────────────────────────────┐
│ 1. parsePosition("e2") → Position(6, 4)                       │
│ 2. parsePosition("e4") → Position(4, 4)                       │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 3. Game::makeMove(Position(6,4), Position(4,4))              │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 4. board->getPiece(Position(6,4))                            │
│    Returns: White Pawn                                        │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 5. Check: pawn->getColor() == currentTurn?                   │
│    White == White → ✓                                         │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 6. pawn->canMove(from, to, board)                            │
│    → PawnMovement::canMove(...)                              │
│    → Checks: 2 squares forward from start row?               │
│    → Path clear? (e3 empty)                                  │
│    → Returns: true ✓                                          │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 7. board->movePiece(from, to)                                │
│    → grid[4][4] = std::move(grid[6][4])                      │
│    → grid[6][4] automatically becomes nullptr                │
│    → pawn->setHasMoved(true)                                 │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 8. Check: board->isInCheck(WHITE)?                           │
│    → board->findKing(WHITE) → Position(7, 4)                 │
│    → board->isSquareUnderAttack(kingPos, BLACK)              │
│    → Scan all black pieces for attacks                       │
│    → Returns: false (not in check)                           │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 9. Game::switchTurn()                                         │
│    → currentTurn = BLACK                                      │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
┌───────────────────────────────────────────────────────────────┐
│ 10. Game::updateGameState()                                   │
│     → board->isInCheck(BLACK)? → false                        │
│     → board->hasValidMoves(BLACK)? → true                     │
│     → state remains ACTIVE                                    │
└───────────────────────┬───────────────────────────────────────┘
                        ▼
                  Loop continues
                  Black's turn
```

---

## File Structure & Pattern Mapping

```
ChessGame/
├── main.cpp                      # Entry point, game loop
│
├── include/
│   ├── Position.h                # Helper struct
│   ├── Game.h                    # Game controller + State Enum
│   ├── Player.h                  # Player data
│   ├── Board.h                   # SINGLETON PATTERN
│   ├── Piece.h                   # Base + concrete pieces
│   ├── PieceFactory.h            # FACTORY PATTERN
│   └── MovementStrategy.h        # STRATEGY PATTERN
│
└── src/
    ├── Game.cpp                  # Game logic + Enum usage
    ├── Player.cpp                # Player implementation
    ├── Board.cpp                 # SINGLETON implementation
    ├── Piece.cpp                 # Piece classes
    ├── PieceFactory.cpp          # FACTORY implementation
    └── MovementStrategy.cpp      # STRATEGY implementation

Pattern Distribution:
├─ Strategy Pattern:  MovementStrategy.h/cpp, Piece.h/cpp
├─ Singleton Pattern: Board.h/cpp
├─ Factory Pattern:   PieceFactory.h/cpp
└─ Enum Pattern:      Game.h/cpp (GameState enum)
```

---

## Interview Discussion Points

### 1. Why Strategy for Movement?
- **Problem:** Each piece moves differently
- **Solution:** Encapsulate each algorithm in separate class
- **Benefit:** Easy to add new pieces without modifying existing code

### 2. Why Singleton for Board?
- **Problem:** Only one board should exist
- **Solution:** Private constructor + static instance
- **Trade-off:** Global state can make testing harder
- **Alternative:** Dependency Injection for better testability

### 3. Why Factory for Pieces?
- **Problem:** Complex initialization (piece + strategy)
- **Solution:** Centralized creation logic
- **Benefit:** Consistent, error-free piece creation

### 4. What's Missing for Production?
- ❌ Castling
- ❌ En passant
- ❌ Pawn promotion
- ❌ Move history (for undo/redo)
- ❌ Save/Load game
- ❌ Time controls
- ❌ AI opponent

### 5. Potential Improvements?
- **Command Pattern:** For undo/redo moves
- **Observer Pattern:** For UI updates, logging
- **Memento Pattern:** For game state saving
- **Builder Pattern:** For complex game setup

---

**End of Code Flow Documentation**

