#ifndef TICTACBOARD_H
#define TICTACBOARD_H
#include "Enum.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// main board class for Tic Tac Toe
class Board {
    // Friend declarations for auxiliary functions -------------------------------
    friend std::ostream &print(std::ostream &os, const Board &board);
    friend int static_eval(const Board &board, const Mark &maximizing_player);
    friend std::vector<Move> gen_movelist(const Board &board);

    // type alias for internal board representation
    using Boardrep = std::vector<std::vector<Mark>>;

    // Interface --------------------------------------------------------------------------------------
    public:
    // Constructors ----------------------------------------------------------
    // Default constructor: blank 3x3 board. 
    Board() = default;

    // initialize square board with a side length value
    Board(const unsigned &length) : internal_board(length), side_length(length)
    {
        if (length > 0 && length <= max_length) {
            for (auto &row : internal_board) {
                for (unsigned num = 1; num <= length; ++num) {
                    row.push_back(Mark::blank);
                }
            }
        }
        else {
            throw std::runtime_error("A Board was initialized to a value that is less than or equal to 0, or larger than Board::max_length! length == " + std::to_string(length));
        }
    }

    // Public Methods ------------------------------------------------------
    bool play_turn(const std::string &algeb_move);
    bool play_turn(const Move &plrmove);

    // undo last played turn on board.
    bool undo_move(const int num_moves = 1);

    // returns the player of the current turn
    const Mark &current_player() const { return turn_player; }

    // total number of marks drawn on the board (rounds)
    int moves_played() const { return Xmove_num + Omove_num; }

    // winner of the game (blank if game status is draw or ongoing);
    const Mark &winner() const { return winning_player; }

    // present status of the game (ongoing, draw, winner declared)
    inline GameState gamestatus() const;

    // Implementation ---------------------------------------------------------------------
    private:
    // maximum length that the board can be initialized to
    static constexpr int max_length {26};

    // in-class initialize 3x3 board with blank spaces. this vector should always be square (both vectors equal in size)
    Boardrep internal_board {    {Mark::blank, Mark::blank, Mark::blank}, // bottom row of board
                                 {Mark::blank, Mark::blank, Mark::blank}, // middle row of board
                                 {Mark::blank, Mark::blank, Mark::blank}    }; // top row of board

    // side length of board (3 by default contructor)
    const Boardrep::size_type side_length = internal_board.size();

    // keep track of the moves played on the board so far.
    std::vector<Move> played_moves;

    // keep track of the number of moves played by each player on this board (max each should reach is half the number of spaces on the board)
    int Xmove_num = 0;
    int Omove_num = 0;

    // start with cross as the first player (turn_player means player of current turn)
    Mark turn_player = Mark::cross;

    // the winner of the game, value is blank if no winner is declared (tie, ongoing)
    Mark winning_player = Mark::blank;

    // Private Methods -------------------------------------------------
    bool board_full() const { return (side_length * side_length == Xmove_num + Omove_num); }

    // called by play_turn() to place the mark of the current player at the specified coordinate.
    bool place_mark(const unsigned &row, const unsigned &col);

    // return true if coordinate point exists within the bounds of the board, and the space is unoccupied
    bool valid_point(const unsigned &row, const unsigned &col) const
    {      
        return (row < side_length && col < side_length && internal_board[row][col] == Mark::blank);
    }
    bool valid_move(const Move &move) const
    {      
        return valid_point(move.row() - 1, move.col() - 1);
    }

    // return true if the mark passed as an argument has a winning row.
    bool check_win(const Mark &m) const;

    bool check_recent_win(const Mark &m) const;
};


inline GameState Board::gamestatus() const
{
    if (winning_player == Mark::error || turn_player == Mark::error) {
        return GameState::error;
    }
    else {
        if (winning_player == Mark::blank) {
            if (board_full()) {
                return GameState::draw;
            }
            else {
                return GameState::ongoing;
            }
        }
        else {
            return GameState::winner;
        }
    }
}


#endif