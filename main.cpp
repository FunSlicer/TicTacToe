#include <iostream>
using std::cin; using std::cout;
#include <string>
using std::string;
#include "Board.h"
int main()
{
    TicTacBoard testboard
    ({  {Mark::blank, Mark::blank, Mark::blank}, 
        {Mark::blank, Mark::blank, Mark::blank}, 
        {Mark::blank, Mark::blank, Mark::blank}  });
    print(cout, testboard);
    return 0;
}