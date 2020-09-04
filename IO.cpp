#include "Board.h"
#include "Enum.h"
#include "IO.h"
#include <iostream>

/* 
IDEAL ILLUSTRATION FOR PRINT
      |     |      Type1                                 type1: 5' ', 1'|' & 5' ', 1'|', 5' ' 1'\n'. 5blank, 1line, 5blank, 1line, 5blank
3  -  |  -  |  -   Type2
 _____|_____|_____ Type3
      |     |      Type1
2  -  |  -  |  -   Type2
 _____|_____|_____ Type3
      |     |      Type1
1  -  |  -  |  -   Type2
      |     |      Type1
   A(1)  B(2)  C(3)
*/

std::ostream &print(std::ostream &os, const Board &board)
{
    const int max_digits = num_of_digits(board.side_length);
    for (auto row_iter = board.internal_board.end() - 1; row_iter != board.internal_board.begin() - 1; --row_iter) {
        // print number for vertical axis
        const auto vert_pos = row_iter - board.internal_board.begin() + 1;
        os << std::string(max_digits - num_of_digits(vert_pos), ' ') << vert_pos << ' ';

        for (auto col_iter = row_iter->begin(); col_iter != row_iter->end(); ++col_iter) {
            // if on final column in row
            if (col_iter != (row_iter->end() - 1)) {
                os << marktochar(*col_iter) << ' ';
            }
            else {
                os << marktochar(*col_iter) << '\n';
            }
        }
    }
    // print horizontal number row
    os << std::string(max_digits + 1, ' ');
    auto row_size = board.internal_board.back().size();
    for (decltype(row_size) col_num = 1; col_num <= row_size; ++col_num) {
        os << numtolet(col_num) << ((col_num == row_size) ? '\n' : ' ');
    }
    return os;
}

std::ostream &print(std::ostream &os, const Move &mv, const bool algeb)
{
    if (algeb) {
        os << move_to_algeb(mv);
    }
    else {
        os << '(' << mv.col() << ", " << mv.row() << ')';
    }
    return os; 
}