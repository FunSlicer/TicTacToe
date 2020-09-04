#include "Board.h"
#include "IO.h"
#include <iostream>
#include <stdexcept>
#include <vector>

// interprets algebraic notation, and passes intended coordinate to place_mark. return true if able to successfully place a mark on an empty spot.
bool Board::play_turn(const std::string &algeb_move) { return play_turn(algeb_to_move(algeb_move)); }

bool Board::play_turn(const Move &plrmove)
{ 
    if (place_mark(plrmove.row() - 1, plrmove.col() - 1)) {
        played_moves.push_back(plrmove);
        // increment move number for specific player and check for win only if that player has placed their mark on the board enough times for a win to be possible
        switch (turn_player) {
            case Mark::circle:
                ++Omove_num;
                if (Omove_num >= side_length && check_recent_win(turn_player)) {
                    winning_player = turn_player;
                }
                // assign for player of next turn
                turn_player = Mark::cross;
            break;

            case Mark::cross:
                ++Xmove_num;
                if (Xmove_num >= side_length && check_recent_win(turn_player)) {
                    winning_player = turn_player;
                }
                // assign for player of next turn
                turn_player = Mark::circle;
            break;

            default:
                throw std::runtime_error("In Board::place_mark(): turn_player was Mark::blank or Mark::error.");
            break;
        }
        return true;
    }
    return false;
}

bool Board::place_mark(const unsigned &row, const unsigned &col)
{
    // check if game is ongoing, throw error if called while game is halted (draw, winner, error)
    if (gamestatus() == GameState::ongoing) {
        // check if spot is occupied - return false if it is 
        if (valid_point(row, col)) {       
            // place mark on an unoccupied space
            internal_board[row][col] = turn_player;
            return true;
        }
        else {
            return false;
        }
    }
    else {
        throw std::runtime_error("In Board::place_mark(): function called while gamestatus() is not ongoing.");
    }
}
/*
bool Board::place_mark(const unsigned &row, const unsigned &col)
{
    // check if game is ongoing, throw error if called while game is halted (draw, winner, error)
    if (gamestatus() == GameState::ongoing) {
        // check if spot is occupied - return false if it is 
        if (valid_point(row, col)) {       
            // place mark on an unoccupied space
            internal_board[row][col] = turn_player;
            // increment move number for specific player and check for win only if that player has placed their mark on the board enough times to get a row
            switch (turn_player) {
                case Mark::circle:
                    ++Omove_num;
                    if (Omove_num >= side_length && check_recent_win(turn_player)) {
                        winning_player = turn_player;
                    }
                    // assign for player of next turn
                    turn_player = Mark::cross;
                break;

                case Mark::cross:
                    ++Xmove_num;
                    if (Xmove_num >= side_length && check_recent_win(turn_player)) {
                        winning_player = turn_player;
                    }
                    turn_player = Mark::circle;
                break;

                default:
                    throw std::runtime_error("In Board::place_mark(): turn_player was Mark::blank.");
                break;
            }
            return true;  
        }
        else {
            return false;
        }
    }
    
}
*/
bool Board::undo_move(const int num_moves)
{
    // **check for errors before doing anything and throw
    if (gamestatus() == GameState::error) {
        throw std::runtime_error("Error in function Board::undo_move(): gamestatus() is at an error state!");
    }
    if (num_moves <= 0 || num_moves > played_moves.size()) {
        throw std::runtime_error("Error in function Board::undo_move(): num_moves outside of range (0, played_moves.size()]!\nnum_moves == " + std::to_string(num_moves) + "\nplayed_moves.size() == " + std::to_string(played_moves.size()));
    }

    if (played_moves.size() > 0) {
        for (int n = 1; n <= num_moves; ++n) {
            // decrement move count for player of last turn, switch turn_player to player of last turn
            switch (turn_player) {
                case Mark::circle:
                    turn_player = Mark::cross;
                    --Xmove_num;
                break;

                case Mark::cross:
                    turn_player = Mark::circle;
                    --Omove_num;
                break;

                default:
                    throw std::runtime_error("In Board::undo_move(): next player was Mark::blank or error.");
                break;
            }
            internal_board[played_moves.back().row() - 1][played_moves.back().col() - 1] = Mark::blank;
            if (gamestatus() == GameState::winner && winning_player == turn_player && !check_recent_win(turn_player)) {
                winning_player = Mark::blank;
            }
            played_moves.pop_back();
        }
        return true;
    }
    else {
        return false;
    }
}
/*
// check for three (board length) in a row of the mark passed as an argument (only start calling this function when Board::turn_number >= 3 or side length size)
bool Board::check_win(const Mark &m) const
{
    // horizontal win check
    for (const auto &row : internal_board) {
        bool row_win = true;
        for (const auto &col : row) {
            // if a non-matching mark is encountered on this row, check next row
            if (col != m) {
                row_win = false;
                break;
            }
        }
        if (row_win) {
            #ifdef DEBUG_MODE
            std::cout << "horizontal win!" << std::endl;
            #endif
            return true;
        }
    }

    // vertical win check
    // stay on first row, for each iteration check a column on each row
    for (auto col_iter = internal_board.front().begin(); col_iter != internal_board.front().end(); ++col_iter) {
        // horizontal index position of columns being checked on each row
        const auto pos = std::distance(internal_board.front().begin(), col_iter);
        bool col_win = true;
        for (const auto &row : internal_board) {
            if (row[pos] != m) {
                col_win = false;
                break;
            }
        }
        if (col_win) {
            #ifdef DEBUG_MODE
            std::cout << "vertical win!" << std::endl;
            #endif
            return true;
        }      
    }
    
    // diagonal win check
    // iterate through each row, use a fwd subscript and rev subscript on each column to do both diagonals in one loop
    decltype(internal_board.size()) fwd_index = 0, rev_index = internal_board.size() - 1;
    bool fwd_win = true, rev_win = true;
    for (const auto &row : internal_board) {
        #ifdef DEBUG_MODE
        std::cout << "row[" << fwd_index << "] = " << marktochar(row[fwd_index]) << std::endl;
        std::cout << "row[" << rev_index << "] = " << marktochar(row[rev_index]) << std::endl;
        #endif
        if (fwd_win && row[fwd_index] != m) {
            #ifdef DEBUG_MODE
            std::cout << "fwd_win = false" << std::endl;
            #endif
            fwd_win = false;
        }
        if (rev_win && row[rev_index] != m) {
            #ifdef DEBUG_MODE
            std::cout << "rev_win = false" << std::endl;
            #endif
            rev_win = false;
        }
        if (!fwd_win && !rev_win) {
            break;
        }
        ++fwd_index;
        --rev_index;
    }
    if (fwd_win || rev_win) {
        #ifdef DEBUG_MODE
        std::cout << "diagonal win!" << std::endl;
        #endif
        return true;
    }
    else {
        #ifdef DEBUG_MODE
        std::cout << "no win detected!" << std::endl;
        #endif
        return false;
    }
}
*/
bool Board::check_recent_win(const Mark &m) const
{
    const Move &last_mv = played_moves.back();
    // check horizontal win on row where most recent move was made.
    bool hori_win = true;
    for (const auto &col : internal_board[last_mv.row() - 1]) {
        if (col != m) {
            hori_win = false;
            break;
        }
    }
    if (hori_win) {
        return true;
    }

    // check vertical win on column where most recent move was made.
    bool vert_win = true;
    for (const auto &row : internal_board) {
        if (row[last_mv.col() - 1] != m) {
            vert_win = false;
            break;
        }
    }
    if (vert_win) {
        return true;
    }

    // check diagonal win if the most recent move lies on either of the two diagonal lines on a given board
    // from bottom-left to top-right i.e. '/'
    if (last_mv.col() == last_mv.row()) {
        bool BLTR_win = true;
        int col = 0;
        for (const auto &row : internal_board) {
            if (row[col++] != m) {
                BLTR_win = false;
                break;
            }
        }
        if (BLTR_win) {
            return true;
        }
    }
    // from bottom-right to top-left i.e. '\'
    if (last_mv.col() + last_mv.row() == side_length + 1) {
        bool TLBR_win = true;
        int col = side_length - 1;
        for (const auto &row :internal_board) {
            if (row[col--] != m) {
                TLBR_win = false;
                break;
            }   
        }
        if (TLBR_win) {
            return true;
        }
    }
    return false;
}