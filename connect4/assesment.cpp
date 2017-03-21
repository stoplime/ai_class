#include "assesments.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

namespace steffen_space{
    
    assesment::assesment(int width, int height, int connect)
    : assesment(width, height, connect, grid_state(width, height), 0)
    {}

    assesment::assesment(int width, int height, int connect, grid_state init_board, int weights)
    : state_space(init_board)
    {
        assert(connect > 3);
        this->width = width;
        this->height = height;
        this->connect = connect;
        ai_connect_weight = std::vector<float>(connect-2, 0);
        opponents_connect_weight = std::vector<float>(connect-2, 0);
        current_grid = init_board;
        max_depth = 2;
        srand(time(NULL));
        update_weights(weights);
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

    void assesment::update_weights(int type){
        ai_weight = 1;
        opponent_weight = -connect;
        switch (type)
        {
        case 1:
            for (int i = 0; i < connect-2; i++){
                ai_connect_weight[i] = i*connect*connect;
                opponents_connect_weight[i] = -i*connect;
            }
            break;
        case 2:
            for (int i = 0; i < connect-2; i++){
                ai_connect_weight[i] = i+1;
                opponents_connect_weight[i] = (i+1)*(-connect);
            }
            break;
        case 3:
            for (int i = 0; i < connect-2; i++){
                ai_connect_weight[i] = (i+1)*(connect);
                opponents_connect_weight[i] = -(i+1);
            }
            break;
        default:
            for (int i = 0; i < connect-2; i++){
                ai_connect_weight[i] = i*connect;
                opponents_connect_weight[i] = i*connect*(-connect);
            }
            break;
        }
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
                if(value != empty_value && ++count >= connect){
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
                if(value != empty_value && ++count >= connect){
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
        while(col < (int)height && row < (int)width){
            char value = grid[row][col];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(value != empty_value && ++count >= connect){
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
        while(col < (int)height && row >= 0){
            char value = grid[row][col];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(value != empty_value && ++count >= connect){
                    return true;
                }
            }
            --row;
            ++col;
        }
        
        // there is no 4 in a row
        return false;
    }
    
    float assesment::heuristics(grid_state input_grid, char ai_piece){
        // TODO: evaluate the current board_state
        std::vector< std::vector<char> > grid = input_grid.get_grid();

        float total_value = 0;
        // check for three + empty combinations
        // vertical
        std::vector< std::vector<int> > v_filter(1, std::vector<int>(connect, 1));
        total_value += filter_assesment(grid, v_filter, ai_piece);
        
        // horizontal
        std::vector< std::vector<int> > h_filter(connect, std::vector<int>(1, 1));
        total_value += filter_assesment(grid, h_filter, ai_piece);

        // diagonal bottom_left to top_right
        std::vector< std::vector<int> > d1_filter(connect, std::vector<int>(connect, 0));
        for (int i = 0; i < connect; i++){
            d1_filter[i][i] = 1;
        }
        total_value += filter_assesment(grid, d1_filter, ai_piece);

        // diagonal bottom_right to top_left
        std::vector< std::vector<int> > d2_filter(connect, std::vector<int>(connect, 0));
        for (int i = 0; i < connect; i++){
            d2_filter[connect-(i+1)][i] = 1;
        }
        total_value += filter_assesment(grid, d2_filter, ai_piece);
        // std::cout << "heuristic: " << total_value << std::endl;

        return total_value;
    }

    float assesment::filter_assesment(std::vector< std::vector<char> > grid, std::vector< std::vector<int> > filter, char ai_piece){
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
                if(filter_value == connect*ai_weight){
                    std::cout << "ERROR: utility not catch 4 in row" << std::endl;
                    return 1000000;
                }
                else if(filter_value == connect*opponent_weight){
                    std::cout << "ERROR: utility not catch 4 in row" << std::endl;
                    return -100000;
                }
                for (int connect_iter = 0; connect_iter < connect-2; connect_iter++){
                    if (filter_value == (connect-(connect_iter+1))*ai_weight){
                        total_value += ai_connect_weight[connect_iter];
                    }
                    else if (filter_value == (connect-(connect_iter+1))*opponent_weight){
                        total_value += opponents_connect_weight[connect_iter]; //*/ai_connect_weight[connect_iter]*(-connect);
                    }
                }
            }
        }// end of grid iteration
        return total_value;
    }
    
    int assesment::build_state_space(char ai_piece){
        iterations = 0;
        // reset state_space
        state_space = tree_node<grid_state >(current_grid);
        // go through a depth first traversal building up the state_space
        int score = build_state_space_recursive(&state_space, 0, ai_piece, -10000000000, 10000000000);
        std::cout << "iterations: " << iterations << std::endl;
        return score;
    }

    /// build and evaluate the state space recursively
    /// node is the current state being evaluated
    /// current_depth is the depth count increment on deeper levels
    
    int assesment::build_state_space_recursive(tree_node< grid_state >* node, int current_depth, char ai_piece, float alpha, float beta){
        bool is_max = current_depth %2 == 0;
        grid_state* grid = &(node->get_data());

        // check depth
        if (current_depth >= max_depth)
        {
            // evaluate heuristics
            grid->set_score(this->heuristics(*grid, ai_piece));
            // std::cout << grid->to_string();
            // std::cout << "heuristic: " << grid->get_score() << std::endl;
            return grid->get_input();
        }

        // figure out whose turn it is
        char turn = '.';
        if (is_max){
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
        float& minimax_thresh = (is_max)? (alpha): (beta);
        int minimax_child = -1;

        // build children in optimized order
        int mid = ((width%2 == 0)? (width): (width-1))/2;
        std::vector<int> children_order = std::vector<int>(width, mid);
        int index_variance = 1;
        for (int i = 1; i < width; i += 2, ++index_variance){
            if(mid+index_variance < width){
                children_order[i] = mid+index_variance;
            }
            if(mid-index_variance >= 0 && i+1 < width){
                children_order[i+1] = mid-index_variance;
            }
        }
        // build children
        for (int index = 0; index < width; index++){
            int i = children_order[index];
            grid_state* temp = new grid_state(*grid);
            int y = temp->set_data(turn, i);
            int x = temp->get_input();
            if(y == -1){
                continue;
            }

            iterations++;
            // /*
            if(x == -1){
                std::cout << "ERROR: temp input == -1" << std::endl;
            }
            bool eval = this->utility(*temp, x, y);
            if (eval){
                // if it is the ai turn, then win the game
                if (is_max){
                    temp->set_score(10000000000);
                }
                // else if it is the opponents turn, then prevent a loss
                else{
                    temp->set_score(-10000000000);
                }
                
                // std::cout << temp->to_string();
                // std::cout << "utility: " << temp->get_score() << std::endl;
            }//*/

            tree_node< grid_state >* new_child = new tree_node< grid_state >();
            new_child->set_data_pointer(temp);
            node->append_child(new_child);
            
            if(!eval){
                build_state_space_recursive(new_child, current_depth+1, ai_piece, alpha, beta);
            }
            // std::cout << std::setw(4*current_depth) << i << ": "<< new_child->get_data().get_score() << std::endl;

            if(un_init){
                minimax_thresh = new_child->get_data().get_score();
                minimax_child = i;
                un_init = false;
            }
            else{
                // if is max
                if(is_max){
                    if (minimax_thresh < new_child->get_data().get_score()){
                        minimax_thresh = new_child->get_data().get_score();
                        minimax_child = i;
                    }
                }
                else{
                    if (minimax_thresh > new_child->get_data().get_score()){
                        minimax_thresh = new_child->get_data().get_score();
                        minimax_child = i;
                    }
                }
            }
            if (alpha >= beta){
                break;
            }
            // new_child->set_data_pointer(NULL);
            // delete new_child;
            // delete temp;
        }
        grid->set_score(minimax_thresh);
        if (minimax_child == -1){
            std::vector<int> possible(1, -1);
            for (int i = 0; i < width; i++){
                if(grid->get_height_empty(i) != -1){
                    possible.push_back(i);
                }
            }
            if(possible.size() > 1){
                minimax_child = possible[(rand() % (possible.size()-1))+1];
            }
        }
        return minimax_child;
    }
}
