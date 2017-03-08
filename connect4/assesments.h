// Evaluates the next choices
#ifndef _ASSESMENTS_H_
#define _ASSESMENTS_H_
#include <array>
#include <algorithm>
#include <cassert>
#include "grid.h"

namespace steffen_space{
    template<int width = 7, int height = 6, int connect = 4>
    class assesment
    {
    private:
        char empty_value = '.';
    public:
        grid_state<width, height> current_grid;
        assesment();
        ~assesment();

        /// Utility assesment on an (x,y)
        /// (x, y) is new piece
        /// turn is the X or O turn
        /// grid is the board being evaluated
        float utility(std::array<std::array<char, height>, width> grid, int x, int y, char turn);
    
    };

/*
    /// calculate the best valued choise based on iterative utility checks
    /// grid is the current board
    /// turn is choice being optimized
    template<class type>
    int expected_value(std::vector< std::vector<type> > grid, type turn){
        // state_space contains a tree like structure of all the possible choices on the grid starting with turn
        state_space states;

    }
    
    short int heuristics(){
        
    }
    */
}

#include "assesment.template"
#endif
