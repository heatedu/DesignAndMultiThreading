# Chess Game - Design Patterns Implementation

## Design Patterns Used

### 1. **Strategy Pattern** - Piece Movements
- **Purpose**: Encapsulate movement algorithms for each piece type
- **Implementation**: 
  - `MovementStrategy` - Abstract base class
  - `KingMovement`, `QueenMovement`, `RookMovement`, etc. - Concrete strategies
  - Each `Piece` has a `MovementStrategy` that defines how it moves
- **Benefit**: Easy to add new pieces or modify movement rules without changing existing code

### 2. **Singleton Pattern** - Board
- **Purpose**: Ensure only one board instance exists
- **Implementation**:
  - Private constructor
  - Static `getInstance()` method
  - Deleted copy constructor and assignment operator
- **Benefit**: Global access point, prevents multiple board instances

### 3. **Factory Pattern** - Piece Creation
- **Purpose**: Centralize piece instantiation
- **Implementation**:
  - `PieceFactory::createPiece(type, color)` creates appropriate piece objects
- **Benefit**: Clean interface for creating pieces, easy to extend

### 4. **Enum for Game State**
- **States**: `ACTIVE`, `SAVED`, `BLACK_WIN`, `WHITE_WIN`, `STALEMATE`
- **Purpose**: Type-safe game state management

## Build Instructions

### Windows (Using CMake)
```bash
mkdir build
cd build
cmake ..
cmake --build .
.\Debug\chess.exe
```

### Using g++ directly
```bash
g++ -std=c++14 main.cpp src/*.cpp -Iinclude -o chess
.\chess.exe
```

## How to Play

1. Moves are entered as: `e2 e4` (from-position to-position)
2. Columns: a-h, Rows: 1-8
3. White pieces: K Q R B N P (uppercase)
4. Black pieces: k q r b n p (lowercase)
5. Type `quit` to exit

## Class Diagram Overview

```
Piece (abstract)
├── King, Queen, Rook, Bishop, Knight, Pawn
└── has-a MovementStrategy

MovementStrategy (abstract)
├── KingMovement
├── QueenMovement
├── RookMovement
├── BishopMovement
├── KnightMovement
└── PawnMovement

Board (Singleton)
├── 8x8 grid of Pieces
└── Move validation, check detection

PieceFactory
└── createPiece(type, color)

Game
├── Board
├── 2 Players
└── GameState enum
```

## Interview Discussion Points

1. **Why Strategy Pattern for movement?**
   - Pieces behave differently, strategy encapsulates this
   - Open/Closed principle - open for extension, closed for modification

2. **Why Singleton for Board?**
   - Only one board in chess
   - Global state management
   - **Trade-off**: Testing can be harder, consider dependency injection alternative

3. **Why Factory for pieces?**
   - Consistent creation interface
   - Encapsulates complex construction logic
   - Easy to add new piece types

4. **What's missing for production?**
   - Castling, en passant
   - Move undo/redo
   - Game persistence
   - Better check validation (currently simplified)
   - AI opponent
   - Time controls

## Time Complexity

- Move validation: O(1) for knight/king, O(n) for sliding pieces
- Check detection: O(n²) where n=8
- Valid moves generation: O(n²)

## Extension Ideas

- Observer Pattern for game events (move made, check, etc.)
- Command Pattern for move history/undo
- Memento Pattern for game state saving

