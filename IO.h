#ifndef TICTACIO_H
#define TICTACIO_H
#include "Board.h"
#include "Enum.h"
#include <cctype>
#include <iostream>
#include <string>

std::ostream &print(std::ostream &os, const Board &board);
std::ostream &print(std::ostream &os, const Move &mv, const bool algeb = true);

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
            return 'E';
            break; 
    }

}

// returns true if passed string is in proper algebraic notation for tic-tac-toe
inline bool correct_syntax(const std::string &input)
{
    using std::string;
    constexpr auto alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    constexpr auto numbers = "1234567890";
    const auto first_alpha_pos = input.find_first_of(alphabet);
    const auto last_alpha_pos = input.find_last_of(alphabet);
    const auto first_num_pos = input.find_first_of(numbers);
    const auto last_num_pos = input.find_last_of(numbers);
    /*
    std::cout << "input == " << input << " input.size() == " << input.size() << std::endl;
    std::cout << "first_alpha_pos == " << first_alpha_pos << " last_alpha_pos == " << last_alpha_pos << " first_num_pos == " << first_num_pos << " last_num_pos == " << last_num_pos << std::endl;
    bool let(first_alpha_pos != string::npos);
    bool num(first_num_pos != string::npos);
    bool onelet(first_alpha_pos == last_alpha_pos);
    bool numafterlet(last_alpha_pos + 1 == first_num_pos);
    bool numislast(last_num_pos == input.size() - 1);
    if (let) {
        std::cerr << "There is at least one letter.\n";
    }
    if (num) {
        std::cerr << "There is at least one number.\n";
    }
    if (onelet) {
        std::cerr << "There is one letter.\n";
    }
    if (numafterlet) {
        std::cerr << "There is a number after the final letter.\n";
    }
    if (numislast) {
        std::cerr << "The string ends in a number.\n";
    }
    if (input.size() >= 2) {
        std::cerr << "input.size() >= 2\n";
    }
    if (input.size() <= 3) {
        std::cerr << "input.size() <= 3\n";
    }
    */
    return (input.size() >= 2 && input.size() <= 3 
        && first_alpha_pos != string::npos && first_num_pos != string::npos
        && first_alpha_pos == last_alpha_pos && last_alpha_pos + 1 == first_num_pos
        && last_num_pos == input.size() - 1);
}

inline char numtolet(const unsigned &let_num, bool uppercase = false)
{
    const std::string alphabet = ((uppercase) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "abcdefghijklmnopqrstuvwxyz");
    if (let_num <= 26) {
        return alphabet[let_num - 1];
    }
    else {
        throw std::runtime_error("In function numtolet(): out of range value passed! let_num == " + std::to_string(let_num));
    }
}

inline unsigned lettonum(const char &let)
{
    if (!std::isalpha(let)) {
        throw std::runtime_error("Non-alphabetical char passed to function lettonum.");
    }
    const std::string alphabet = ((std::isupper(let)) ? "ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "abcdefghijklmnopqrstuvwxyz");
    return alphabet.find_first_of(let) + 1;
}

// converts a string denoting algebraic notation into a Move coordinate
inline Move algeb_to_move(const std::string &algebraic)
{
    return Move(lettonum(algebraic.front()), std::stoul(algebraic.substr(1)));
}

// converts a Move into a string denoting algebraic notation to the corresponding coordinate.
inline std::string move_to_algeb(const Move &mv)
{
    return numtolet(mv.col()) + std::to_string(mv.row());
}

// returns a value denoting the number of characters in an int when printed. i.e. number of digits + possible negative sign.
constexpr int num_of_digits(int n)
{
    // account for the negative sign of a negative value
    int ret_val = ((n >= 0) ? 0 : 1);
    while(n != 0) {
        n /= 10;
        ++ret_val;
    }
    return ret_val;
}
#endif