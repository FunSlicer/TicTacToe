#ifndef TICTACENUM_H
#define TICTACENUM_H
#include <climits>

// used to represent players and symbols on board
enum class Mark {blank, circle, cross, error = -1};

// used to represent the status of the game
enum class GameState {ongoing, draw, winner, error = -1};

// used by TicTacBoard class to represent moves made by the player, where [0][0] on the internal board is (1, 1) as a move.
class Move {
    public:
    Move() = default;
    Move(const int &col, const int &row) : x(col), y(row) { }

    const int &col() const { return x; }
    const int &row() const { return y; }

    private:
    int x = 0;
    int y = 0;
};
#endif