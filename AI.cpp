#include "Board.h"
#include "Enum.h"
#include "IO.h"
#include "AI.h"
#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <vector>


// returns the best move for the current player of the board
Move find_best_move(const Board &board) {
    int best_val = -AI_INFINITY;
    Board temp_board = board;
    Move best_move;
    std::vector<Move> movelist(gen_movelist(board));
    static std::chrono::milliseconds total_duration;
    for (const Move &mv : movelist) {
        std::cerr << "Current Move: ";
        print(std::cerr, mv) << '\n';
        temp_board.play_turn(mv);
        auto before_call = std::chrono::high_resolution_clock::now();
        int current_val = alphabeta(temp_board, board.current_player());
        auto after_call = std::chrono::high_resolution_clock::now();

        auto time_duration = std::chrono::duration_cast<std::chrono::milliseconds>(after_call - before_call);
        total_duration += time_duration;

        std::cerr << "Algorithm execution time: " << time_duration.count() << " milliseconds.\n";
        temp_board.undo_move();
        std::cerr << "Value: " << current_val << std::endl;
        if (current_val > best_val) {
            best_move = mv;
            best_val = current_val;
        }
        else if (best_val == current_val) {
            best_move = ((random_int(0, 100) > 50) ? best_move : mv);
        }
    }
    std::cerr << "total duration = " << total_duration.count()  << " milliseconds\n"
    << std::chrono::duration_cast<std::chrono::seconds>(total_duration).count() << " seconds.\n";
    return best_move;
}

int alphabeta(Board &board, const Mark &maximizing_player, int alpha, int beta)
{
    // if we are at a terminal state, evaluate the given board
    if (board.gamestatus() != GameState::ongoing) {
        if (board.gamestatus() == GameState::error) {
            throw std::runtime_error("Error in function minimax(): encountered board at an error state!");
        }
        else {
            return static_eval(board, maximizing_player);
        }
    }

    const bool is_maximizing = (board.current_player() == maximizing_player);
    int best_val = ((is_maximizing) ? -AI_INFINITY : AI_INFINITY);
    std::vector<Move> movelist(gen_movelist(board));
    for (const auto &mv : movelist) {
        board.play_turn(mv);
        if (is_maximizing) {
            best_val = std::max(best_val, alphabeta(board, maximizing_player, alpha, beta));
            alpha = std::max(alpha, best_val);
        }
        else {
            best_val = std::min(best_val, alphabeta(board, maximizing_player, alpha, beta));
            beta = std::min(beta, best_val);
        }
        board.undo_move();
        if (alpha >= beta) {
            break;
        }
    }
    return best_val;
}

int minimax(Board &board, const Mark &maximizing_player)
{
    // if we are at a terminal state, evaluate the given board
    if (board.gamestatus() != GameState::ongoing) {
        if (board.gamestatus() == GameState::error) {
            throw std::runtime_error("Error in function minimax(): encountered board at an error state!");
        }
        else {
            return static_eval(board, maximizing_player);
        }
    }
    
    const bool is_maximizing = (board.current_player() == maximizing_player);
    int best_val = ((is_maximizing) ? -AI_INFINITY : AI_INFINITY);
    std::vector<Move> movelist(gen_movelist(board));
    for (const auto &mv : movelist) {
        board.play_turn(mv);
        if (is_maximizing) {
            best_val = std::max(best_val, minimax(board, maximizing_player));
        }
        else {
            best_val = std::min(best_val, minimax(board, maximizing_player));
        }
        board.undo_move();
    }
    return best_val;
}

std::vector<Move> gen_movelist(const Board &board)
{
    std::vector<Move> movelist;
    for (int row = 1; row <= board.side_length; ++row) {
        for (int col = 1; col <= board.side_length; ++col) {
            Move mv(col, row);
            if (board.valid_move(mv)) {
                movelist.push_back(mv);
            }
        }
    }
    return movelist;
}