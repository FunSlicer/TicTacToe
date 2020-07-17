#include <iostream>
#include <vector>
#include <iterator>

// used to represent players and symbols on board
enum class Mark {blank, circle, cross};
// used to represent the status of the game
enum class GameStatus {ongoing, draw, winner};

class TicTacBoard {
    friend std::ostream &print(std::ostream &os, const TicTacBoard &board);
    public:
    // type alias for internal board representation
    using Boardrep = std::vector<std::vector<Mark>>;
    // Default constructor: blank 3x3 board, X goes first. 
    TicTacBoard() = default;
    // initialize board with a board savestate (define this outside of class body) 
    TicTacBoard(const Boardrep &board_savestate) : board(board_savestate) 
    {
        // **add code to compute which player's turn it is on this board:
        // if number of X == number of O, turn_player = X, 
        // if number of X == number of O + 1, turn_player = O. Else, error.
    }
    // return true if able to successfully place a mark on an empty spot.
    bool play_turn(const unsigned &col, const unsigned &row);
    // returns the player of the current turn
    const Mark &current_player() { return turn_player; }
    // total number of marks drawn on the board (rounds)
    int turns_played() { return Xmove_num + Omove_num; }
    // winner of the game (blank if draw or ongoing);
    const Mark &winner() { return winning_player; }
    // present status of the game (ongoing, draw, winner declared)
    GameStatus game_status();

    private:
    
    // in-class initialize 3x3 board with blank spaces this vector should always be square (both vectors equal in size)
    Boardrep board
    {   {Mark::blank, Mark::blank, Mark::blank}, 
        {Mark::blank, Mark::blank, Mark::blank}, 
        {Mark::blank, Mark::blank, Mark::blank}   };

    // side length of board (3 by default contructor)
    const decltype(board.size()) side_length = board.size();

    // return true if coordinate point exists within the bounds of the board, and the space is unoccupied
    bool valid_point(const unsigned &col, const unsigned &row) const
    {      
        return (col <= side_length && row <= side_length && board[row][col] == Mark::blank);
    }
    bool board_full() { return (side_length * side_length == Xmove_num + Omove_num); }

    bool check_win(const Mark &m);
    // keep track of the number of moves played by each player on this board (max it should reach is the number of spaces on the board)
    unsigned Xmove_num = 0;
    unsigned Omove_num = 0;
    // start with cross as the first player (turn_player means player of current turn)
    Mark turn_player = Mark::cross;
    Mark winning_player = Mark::blank;
};

//std::ostream &print(std::ostream &os, TicTacBoard &board);

inline Mark next_player(const Mark &m)
{
    switch(m) {
        case Mark::cross:
            return Mark::circle;
            break;

        case Mark::circle:
            return Mark::cross;
            break;

        case Mark::blank:
            //error stuff...
            return Mark::blank;
            break;

        default:
            // assert for error I guess...
            break;
    }
}

inline char marktochar(const Mark &m)
{
    switch(m) {
        case Mark::blank:
            return '-';
            break;
        case Mark::circle:
            return 'O';
            break;
        case Mark::cross:
            return 'X';
            break;
        default:
            // error stuff... maybe assert.
            return 'E';
            break; 
    }
}