#pragma once

#include <string>
#include <sstream>

class Position {
public:
    int row;
    int col;

    // Constructor
    Position(int row, int col) : row(row), col(col) {}

    // toString equivalent
    std::string toString() const {
        std::ostringstream out;
        out << "(" << row << ", " << col << ")";
        return out.str();
    }

    // Equality operator (equivalent to Java equals)
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    // Hash function support if needed for unordered_map / unordered_set
    struct Hash {
        std::size_t operator()(const Position& p) const {
            return std::hash<int>()(p.row * 31 + p.col);
        }
    };
};
