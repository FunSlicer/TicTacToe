#include "Board.h"
#include "Game.h"
#include "IO.h"
#include "AI.h"
#include <cassert>
#include <iostream> 
using std::cout; using std::cin;
#include <string>
using std::string;

void tictactoe()
{
    bool valid_input = true;
    do {
        valid_input = true;
        switch (display_menu(cout)) {
            case 1:
                start_game();
                break;

            case 2:
                start_game(Mark::cross);
                break;

            case 3:
                return;
                break;

            default:
                // error: invalid input.
                valid_input = false;
                break;
        }
    } while(!valid_input);
    return;
}

int display_menu(std::ostream &os)
{
    int choice = 0;
    os << "~ Tic Tac Toe - Main Menu ~\n"
        << "Choose an option from the following menu:\n"
        << "1. Play a game against a human opponent\n"
        << "2. Play a game against a computer opponent\n"
        << "3. Quit Game\n";
    cin >> choice;
    return choice;
}

void start_game(const Mark &ai_opponent)
{
    // initialize game board
    int game_size = 3;
    cout << "What size should the board be?\n";
    cin >> game_size;
    Board game(game_size);

    // loop for every round of the game.
    while(game.gamestatus() == GameState::ongoing) {
        #ifndef AI_DEBUG
        print(cout, game);

        if (game.current_player() == ai_opponent) {
            game.play_turn(find_best_move(game));
        }
        else {
            cout << "It is " << marktochar(game.current_player()) << "'s turn. Enter two numbers representing the coordinate of the spot you want to place your mark.\n";
            string input;
            cin >> input;
            if (input == "undo") {
                game.undo_move(2);
                continue;
            }
            // ensure syntax is correct before passing values to function
            if(correct_syntax(input)) {
                // notify user if specified coordinate is occupied.
                if (!game.play_turn(input)) {
                    cout << "Invalid coordinate! Try again!\n";
                    continue;
                }
            }
            else {
                cout << "Invalid input! Position must be specified in Chess algebraic notation. E.g. \"b2\" or \"a3\"\n";
                continue;
            }
        }
        #else
        print(std::cout, game);
        game.play_turn(find_best_move(game));
        #endif
    }
    print(cout, game);
    switch(game.gamestatus()) {
        case GameState::winner:
            cout << marktochar(game.winner()) << " is the winner!"; 
            break;
        case GameState::draw:
            cout << "The game has ended at a draw!";
            break;
        case GameState::error:
            std::cerr << "The game has terminated in an error..";
            break;
        default:
            std::cerr << "This should be impossible. The game has ended, while Board::gamestatus() still returns GameState::ongoing. Asserting for impossible error..";
            assert(game.gamestatus() == GameState::ongoing);
            break;
    }
    return;
}