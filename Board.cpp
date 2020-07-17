#include <iostream>
#include <vector>
#include "Board.h"

bool TicTacBoard::play_turn(const unsigned &col, const unsigned &row)
{
    // check if spot is occupied - return false if it is 
    if (valid_point(col, row) && game_status() == GameStatus::ongoing) {
        // place mark on an unoccupied space
        board[row][col] = turn_player;
        // increment move number for specific player and check for win only if that player has placed their mark on the board enough times to get a row
        switch (turn_player) {
            case Mark::circle:
                ++Omove_num;
                if (Omove_num >= board.size() && check_win(turn_player)) {
                    winning_player = turn_player;
                }
                break;

            case Mark::cross:
                ++Xmove_num;
                if (Xmove_num >= board.size() && check_win(turn_player)) {
                    winning_player = turn_player;
                }
                break;

            default:
            // error stuff... (assert if not even Mark::blank)
                break;
        }
        turn_player = next_player(turn_player);
        return true; 
        
    }
    else {
        // error: occupied coordinate point.
        return false;
    }
}

/* IDEAL ILLUSTRATION FOR PRINT
   1     2     3
      |     |      line type 1: 5 ' ', 1 '|', 5 ' ', 1 '|', 1 '\n'.
1  -  |  -  |  -  
 _____|_____|_____
      |     |     
2  -  |  -  |  -  
 _____|_____|_____
      |     |     
3  -  |  -  |  -  
      |     |     
*/
std::ostream &print(std::ostream &os, const TicTacBoard &game)
{
    for (auto &row : game.board) {
        for (auto col_iter = row.begin(); col_iter != row.end(); ++col_iter) {
            if (col_iter != (row.end() - 1)) {
                os << marktochar(*col_iter) << ' ';
            }
            else {
                os << marktochar(*col_iter) << '\n';
            }
        }
    }
    return os;
}

// check for three (board length) in a row of the mark passed as an argument (only start calling this function when TicTacBoard::turn_number >= 3 or side length size)
inline bool TicTacBoard::check_win(const Mark &m)
{
    // horizontal win check
    for (const auto &row : board) {
        bool row_win = true;
        for (const auto &col : row) {
            // if a non-matching mark is encountered on this row, check next row
            if (col != m) {
                row_win = false;
                break;
            }
        }
        if (row_win) {
            std::cout << "horizontal win!" << std::endl;
            return true;
        }
    }

    // vertical win check
    // stay on first row, for each iteration check a column on each row
    for (auto col_iter = board.front().begin(); col_iter != board.front().end(); ++col_iter) {
        // horizontal index position of columns being checked on each row
        const auto pos = std::distance(board.front().begin(), col_iter);
        bool col_win = true;
        for (const auto &row : board) {
            if (row[pos] != m) {
                col_win = false;
                break;
            }
        }
        if (col_win) {
            std::cout << "vertical win!" << std::endl;
            return true;
        }      
    }
    
    // diagonal win check
    // iterate through each row, use a fwd subscript and rev subscript on each column to do both diagonals in one loop
    decltype(board.size()) fwd_index = 0, rev_index = board.size() - 1;
    bool fwd_win = true, rev_win = true;
    for (const auto &row : board) {
        std::cout << "row[" << fwd_index << "] = " << marktochar(row[fwd_index]) << std::endl;
        std::cout << "row[" << rev_index << "] = " << marktochar(row[rev_index]) << std::endl;
        if (fwd_win && row[fwd_index] != m) {
            std::cout << "fwd_win = false" << std::endl;
            fwd_win = false;
        }
        if (rev_win && row[rev_index] != m) {
            std::cout << "rev_win = false" << std::endl;
            rev_win = false;
        }
        if (!fwd_win && !rev_win) {
            break;
        }
        ++fwd_index;
        --rev_index;
    }
    if (fwd_win || rev_win) {
        std::cout << "diagonal win!" << std::endl;
        return true;
    }
    else {
        std::cout << "no win detected!" << std::endl;
        return false;
    }
}

inline GameStatus TicTacBoard::game_status()
{
    if (winner() == Mark::blank) {
        if (board_full()) {
            return GameStatus::draw;
        }
        else {
            return GameStatus::ongoing;
        }
    }
    else {
        
        return GameStatus::winner;
    }
}