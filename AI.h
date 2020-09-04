#ifndef TICTACAI_H
#define TICTACAI_H
#include "Board.h"
#include <random>
#include <vector>

// infinity for minimax initializations
inline constexpr int AI_INFINITY = INT_MAX;;

// function to return the best move to make on a given board
Move find_best_move(const Board &board);

// function to execute minimax algorithm to return the value of the board given
int minimax(Board &board, const Mark &maximizing_player);

int alphabeta(Board &board, const Mark &maximizing_player, int alpha = -AI_INFINITY, int beta = AI_INFINITY);

// function to generate the available moves in a given board.
std::vector<Move> gen_movelist(const Board &board);

// function to directly return the value of a given board at a non-ongoing, non-error state
inline int static_eval(const Board &board, const Mark &maximizing_player)
{
    constexpr int winning_score = Board::max_length * Board::max_length + 1;
    constexpr int losing_score = -(Board::max_length * Board::max_length) - 1;
    if (board.gamestatus() == GameState::winner) {
        if (board.winner() == maximizing_player) {
            return winning_score - board.moves_played();
        }
        else {
            return losing_score + board.moves_played();
        }
    }
    else {
        if (board.gamestatus() == GameState::draw) {
            return 0;
        }
        else {
            if (board.gamestatus() == GameState::ongoing) {
                std::runtime_error("Error in static_eval(): function called while board has an ongoing game!");
            }
            else {
                std::runtime_error("Error in static_eval(): function called while board has an error state!");
            }
            exit(EXIT_FAILURE);
        }
    }
}

inline int random_int(const int &min, const int &max)
{
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<int> distro(min, max);
    return distro(engine);
}
#endif