// Evaluates the next choices
#ifndef _ASSESMENTS_H_
#define _ASSESMENTS_H_
#include <array>
#include <algorithm>
#include <cassert>
#include "grid.h"
#include "tree_node.h"

namespace steffen_space{
    template<int width = 7, int height = 6, int connect = 4>
    class assesment
    {
    private:
        grid_state<width, height> current_grid;
        char empty_value = '.';
        tree_node<grid_state<width, height> > state_space;
        int max_depth;

    public:
        assesment(grid_state<width, height> board_state);
        ~assesment();

        grid_state<width, height> get_grid();        

        /// Utility assesment on an (x,y)
        /// (x, y) is new piece
        /// turn is the X or O turn
        /// grid is the board being evaluated
        float utility(const grid_state<width, height> input_grid, int x, int y);
        /// Utility on the current_grid
        float utility(int x, int y);
    
        float heuristics(grid_state<width, height> grid);

        void build_state_space();
        void build_state_space_recursive(tree_node< grid_state<width, height> >* node, int current_depth);
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
