# How to Build and Run ChessGame

## ✅ SOLUTION IS READY!

The `ChessGame.sln` is fully configured and ready to run.

## Method 1: Visual Studio (Recommended)

1. **Open Solution:**
   - Double-click `ChessGame.sln`
   - Visual Studio will open the project

2. **Build:**
   - Press `Ctrl+Shift+B` or
   - Menu: `Build > Build Solution`

3. **Run:**
   - Press `F5` (with debugging) or `Ctrl+F5` (without debugging)

## Method 2: CMake Build (Already Tested ✅)

```powershell
cd D:\DesignPatterns\ChessGame
mkdir build
cd build
cmake ..
cmake --build . --config Debug
.\Debug\chess.exe
```

## How to Play

1. **Input Format:** `e2 e4` (from to)
   - Columns: a-h
   - Rows: 1-8

2. **Example Moves:**
   ```
   e2 e4    # White pawn forward
   e7 e5    # Black pawn forward
   g1 f3    # White knight
   quit     # Exit game
   ```

3. **Piece Symbols:**
   - White: `K Q R B N P` (uppercase)
   - Black: `k q r b n p` (lowercase)

## File Structure

```
ChessGame/
├── ChessGame.sln              ← Open this in Visual Studio
├── ChessGame/
│   ├── ChessGame.vcxproj      ← Project file
│   └── main.cpp               ← Entry point
├── include/                   ← All headers
│   ├── Board.h
│   ├── Game.h
│   ├── Piece.h
│   └── ...
├── src/                       ← Implementation files
│   ├── Board.cpp
│   ├── Game.cpp
│   └── ...
└── build/                     ← CMake build output
    └── Debug/
        └── chess.exe          ← Executable
```

## Design Patterns Implemented

1. **Strategy Pattern** - Movement strategies for each piece type
2. **Singleton Pattern** - Single Board instance
3. **Factory Pattern** - Piece creation via PieceFactory
4. **Enum State Management** - GameState tracking

## Tested ✅

Build: **SUCCESS**
Run: **SUCCESS**

Example output:
```
=== Chess Game - Design Patterns Demo ===
Patterns Used:
1. Strategy Pattern - Piece Movement
2. Singleton Pattern - Board
3. Singleton Pattern - Board
4. Enum - Game State Management

  a b c d e f g h
8 r n b q k b n r 8
7 p p p p p p p p 7
6 . . . . . . . . 6
5 . . . . . . . . 5
4 . . . . . . . . 4
3 . . . . . . . . 3
2 P P P P P P P P 2
1 R N B Q K B N R 1
  a b c d e f g h

Alice's turn (White)
Enter move (e.g., 'e2 e4') or 'quit':
```

