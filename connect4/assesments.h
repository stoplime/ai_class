// Evaluates the next choices
#ifndef _ASSESMENTS_H_
#define _ASSESMENTS_H_
#include <vector>
#include <algorithm>
#include <cassert>
#include "grid.h"
#include "tree_node.h"

namespace steffen_space{
    class assesment
    {
    private:
        const char empty_value = '.';

        int width;
        int height;
        int connect;

        grid_state current_grid;
        char ai_piece;
        tree_node<grid_state> state_space;
        int max_depth;

    public:
        assesment(int width, int height, int connect, char ai_piece, grid_state init_board);
        assesment(int width, int height, int connect, char ai_piece);
        //~assesment();

        std::vector< std::vector<char> >& get_grid();
        grid_state& get_grid_state();

        /// Utility assesment on an (x,y)
        /// (x, y) is new piece
        /// turn is the X or O turn
        /// grid is the board being evaluated
        float utility(grid_state input_grid, int x, int y, int connect);
        float utility(grid_state input_grid, int x, int y);
        float utility(int x, int y);
    
        float heuristics(grid_state input_grid);
        float filter_assesment(std::vector< std::vector<char> > grid, std::vector< std::vector<int> > filter);

        int build_state_space();
        int build_state_space_recursive(tree_node<grid_state>* node, int current_depth);
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

#endif
