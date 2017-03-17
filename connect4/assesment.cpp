#include "assesments.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

namespace steffen_space{
    
    assesment::assesment(int width, int height, int connect, char ai_piece)
    : assesment(width, height, connect, ai_piece, grid_state(width, height))
    {}

    assesment::assesment(int width, int height, int connect, char ai_piece, grid_state init_board)
    : state_space(init_board)
    {
        this->width = width;
        this->height = height;
        this->connect = connect;
        this->ai_piece = ai_piece;
        current_grid = init_board.copy();
        max_depth = 2;
        srand(time(NULL));
        update_weights();
    }

    int assesment::get_max_depth(){
        return max_depth;
    }

    void assesment::set_max_depth(int value){
        max_depth = value;
    }
    
    std::vector< std::vector<char> >& assesment::get_grid(){
        return current_grid.get_grid();
    }

    grid_state& assesment::get_grid_state(){
        return current_grid;
    }

    void assesment::update_weights(){
        ai_weight = 1;
        opponent_weight = -connect;
        ai_4_weight = connect*connect*connect*connect;
        opponent_4_weight = -connect*connect*connect*connect*connect;
        ai_3_weight = connect*connect;
        opponent_3_weight = -connect*connect*connect;
        ai_2_weight = 1;
        opponent_2_weight = -connect;
    }
    
    bool assesment::utility(int x, int y){
        return this->utility(this->current_grid, x, y, this->connect);
    }

    bool assesment::utility(grid_state input_grid, int x, int y){
        return this->utility(input_grid, x, y, this->connect);
    }
    
    bool assesment::utility(grid_state input_grid, int x, int y, int connect){
        std::vector< std::vector<char> > grid = input_grid.get_grid();
        assert(width > 0);
        // check invalid moves
        if (y < 0 || y >= height || x < 0 || x >= width){
            return false;
        }

        // check vertical
        char prev_value = 0;
        int count = 0;
        for (int i = 0; i < height; i++){
            char value = grid[x][i];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(++count >= connect && value != empty_value){
                    return true;
                }
            }
        }

        // check horizontal
        prev_value = 0;
        count = 0;
        for (int i = 0; i < width; i++){
            char value = grid[i][y];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(++count >= connect && value != empty_value){
                    return true;
                }
            }
        }

        // check diagonals
        // start from bottom-left
        count = 0;
        prev_value = 0;
        int Min = std::min(x,y);
        int row = x - Min;
        int col = y - Min;
        while(row < (int)width && col < (int)height){
            char value = grid[row][col];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(++count >= connect && value != empty_value){
                    return true;
                }
            }
            ++row;
            ++col;
        }

        // start from bottom-right
        count = 0;
        prev_value = 0;
        Min = std::min((int)width-1 - x, y);
        row = x + Min;
        col = y - Min;
        while(row >= 0 && col < (int)height){
            char value = grid[row][col];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(++count >= connect && value != empty_value){
                    return true;
                }
            }
            --row;
            ++col;
        }
        
        // there is no 4 in a row
        return false;
    }
    
    float assesment::heuristics(grid_state input_grid){
        // TODO: evaluate the current board_state
        std::vector< std::vector<char> > grid = input_grid.get_grid();

        float total_value = 0;
        // check for three + empty combinations
        // vertical
        std::vector< std::vector<int> > v_filter(1, std::vector<int>(connect, 1));
        total_value += filter_assesment(grid, v_filter);
        
        // horizontal
        std::vector< std::vector<int> > h_filter(connect, std::vector<int>(1, 1));
        total_value += filter_assesment(grid, h_filter);

        // diagonal bottom_left to top_right
        std::vector< std::vector<int> > d1_filter(connect, std::vector<int>(connect, 0));
        for (int i = 0; i < connect; i++){
            d1_filter[i][i] = 1;
        }
        total_value += filter_assesment(grid, d1_filter);

        // diagonal bottom_right to top_left
        std::vector< std::vector<int> > d2_filter(connect, std::vector<int>(connect, 0));
        for (int i = 0; i < connect; i++){
            d2_filter[connect-(i+1)][i] = 1;
        }
        total_value += filter_assesment(grid, d2_filter);
        // std::cout << "heuristic: " << total_value << std::endl;

        return total_value;
    }

    float assesment::filter_assesment(std::vector< std::vector<char> > grid, std::vector< std::vector<int> > filter){
        int filter_width = filter.size();
        assert(filter_width > 0); 
        int filter_height = filter[0].size();
        assert(filter_height > 0);

        float total_value = 0;
        // grid iterations
        for (int i = 0; i <= width-filter_width; i++){
            for (int j = 0; j <= height-filter_height; j++){
                int filter_value = 0;
                // filter iterations
                for (int f_i = 0; f_i < filter_width; f_i++){
                    for (int f_j = 0; f_j < filter_height; f_j++){
                        if (filter[f_i][f_j] == 1)
                        {
                            if (grid[i+f_i][j+f_j] == '.')
                            {}
                            else if (grid[i+f_i][j+f_j] == ai_piece){
                                filter_value += ai_weight;
                            }
                            else{
                                filter_value += opponent_weight;
                            }
                        }
                        
                    }
                }// end of filter iteration
                // check for four in a row
                if(filter_value == 4*ai_weight){
                    total_value += ai_4_weight;
                }
                else if(filter_value == 4*opponent_weight){
                    total_value += opponent_4_weight;
                }
                // check for three in a row
                if(filter_value == 3*ai_weight){
                    total_value += ai_3_weight;
                }
                else if(filter_value == 3*opponent_weight){
                    total_value += opponent_3_weight;
                }
                // check for two in a row
                if(filter_value == 2*ai_weight){
                    total_value += ai_2_weight;
                }
                else if(filter_value == 2*opponent_weight){
                    total_value += opponent_2_weight;
                }
            }
        }// end of grid iteration
        return total_value;
    }
    
    int assesment::build_state_space(){
        // reset state_space
        state_space = tree_node<grid_state >(current_grid);
        // go through a depth first traversal building up the state_space
        return build_state_space_recursive(&state_space, 0, -10000000000, 10000000000);
    }

    /// build and evaluate the state space recursively
    /// node is the current state being evaluated
    /// current_depth is the depth count
    
    int assesment::build_state_space_recursive(tree_node< grid_state >* node, int current_depth, float alpha, float beta){
        grid_state* grid = &(node->get_data());
        // std::cout << "" << grid->get_score() << std::endl;
        bool eval = this->utility(*grid, grid->get_input(), grid->get_height_empty(grid->get_input()));
        // if there is a 4 in a row, skip children
        if (eval){
            // if it is the ai turn, then win the game
            if (current_depth %2 == 0){
                grid->set_score(-10000000);
            }
            // else if it is the opponents turn, then prevent a loss
            else{
                grid->set_score(10000000);
            }
            
            std::cout << grid->to_string();
            std::cout << "utility: " << grid->get_score() << std::endl;
            return grid->get_input();
        }
        // check depth
        if (current_depth >= max_depth)
        {
            // evaluate heuristics
            grid->set_score(this->heuristics(*grid));
            // std::cout << grid->to_string();
            // std::cout << "heuristic: " << grid->get_score() << std::endl;
            return grid->get_input();
        }

        // figure out whose turn it is
        char turn = '.';
        if (current_depth %2 == 0){
            turn = ai_piece;
        }
        else{
            if (ai_piece == 'X'){
                turn = 'O';
            }
            else{
                turn = 'X';
            }
        }
        bool un_init = true;
        float minimax_thresh;
        if (current_depth %2 == 0){
            minimax_thresh = alpha;
        }
        else{
            minimax_thresh = beta;
        }
        int minimax_child = -1;

        // build children in optimized order
        int mid = ((width%2 == 0)? (width): (width-1))/2;
        std::vector<int> children_order = std::vector<int>(width, mid);
        int index_variance = 1;
        for (int i = 1; i < width; i += 2, ++index_variance){
            if(mid+index_variance < width){
                children_order[i] = mid+index_variance;
            }
            if(mid-index_variance >= 0){
                children_order[i+1] = mid-index_variance;
            }
        }
        // build children
        for (int index = 0; index < width; index++){
            int i = children_order[index];
            grid_state* temp = new grid_state();
            *temp = grid->copy();
            int y = temp->set_data(turn, i);
            if(y == -1){
                continue;
            }

            tree_node< grid_state >* new_child = new tree_node< grid_state >();
            new_child->set_data_pointer(temp);
            node->append_child(new_child);
            
            build_state_space_recursive(new_child, current_depth+1, alpha, beta);
            // std::cout << std::setw(2*current_depth) << new_child->get_data().get_score() << std::endl;

            if(un_init){
                minimax_thresh = new_child->get_data().get_score();
                minimax_child = i;
                un_init = false;
            }
            else{
                // if is max
                if(current_depth %2 == 0){
                    if (minimax_thresh < new_child->get_data().get_score()){
                        minimax_thresh = new_child->get_data().get_score();
                        minimax_child = i;
                    }
                    if(beta <= minimax_thresh){
                        break;
                    }
                }
                else{
                    if (minimax_thresh > new_child->get_data().get_score()){
                        minimax_thresh = new_child->get_data().get_score();
                        minimax_child = i;
                    }
                    if(minimax_thresh <= alpha){
                        break;
                    }
                }
            }
        }
        grid->set_score(minimax_thresh);
        if (minimax_child == -1){
            minimax_child = rand() % connect;
        }
        return minimax_child;
    }
}
