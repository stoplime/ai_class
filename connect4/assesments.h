// Evaluates the next choices
#ifndef _ASSESMENTS_H_
#define _ASSESMENTS_H_
#include <vector>
#include <cassert>

namespace steffen_space{
    char empty_value = '.';
    int connect_length = 4;

    /// Utility assesment on an (x,y)
    /// (x, y) is new piece
    /// turn is the X or O turn
    /// grid is the board being evaluated
    template<class type>
    int utility(std::vector< std::vector<type> > grid, int x, int y, type turn);
/*
    short int expected_value(){
        
    }

    short int heuristics(){
        
    }
    */
}
#endif
