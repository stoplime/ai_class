// Evaluates the next choices
#ifndef _ASSESMENTS_H_
#define _ASSESMENTS_H_
#include <vector>
#include <cassert>

namespace steffen_space{
    char empty_value = '.';
    int connect_length = 4;

    /// Utility assesment on an (x,y)
    /// 
    template<class type>
    short int utility(vector< vector<type> > grid, int x, int y, type turn){
        assert(grid.size() > 0);
        // check vertical
        for(size_t i=0; i < grid.size(); ++i){
            type prev_value = '';
            int v_count = 0;
            for(size_t j=0; j < grid[i].size(); ++j){
                type value = grid[i][j].get_data();
                if(value != prev_value){
                    v_count = 1;
                    prev_value = value;
                }
                else{
                    if(++v_count >= 4 && value != empty_value){
                        if(value == turn){
                            return 1000;
                        }
                        else{
                            return -1000;
                        }
                    }
                }
            }
        }

        // check horizontal
        for(size_t j=0; j < grid[0].size(); ++j){
            type prev_value = '';
            int h_count = 0;
            for(size_t i=0; i < grid.size(); ++i){
                type value = grid[i][j].get_data();
                if(value != prev_value){
                    h_count = 1;
                    prev_value = value;
                }
                else{
                    if(++h_count >= 4 && value != empty_value){
                        if(value == turn){
                            return 1000;
                        }
                        else{
                            return -1000;
                        }
                    }
                }
            }
        }

        // check diagonals
        // start from bottom-left -- row side
        for(size_t i=0; i < grid.size() - connect_length; ++i){
            int count = 0;
            int row = i;
            int col = 0;
            char prev_value = '';
            while(row < grid.size() && col < grid[row].size()){
                type value = grid[row][col].get_data();
                if(value != prev_value){
                    count = 1;
                    prev_value = value;
                }
                else{
                    if(++count >= 4 && value != empty_value){
                        if(value == turn){
                            return 1000;
                        }
                        else{
                            return -1000;
                        }
                    }
                }
                ++row;
                ++col;
            }
        }
    }

    short int expected_value(){
        
    }

    short int heuristics(){
        
    }
}

#endif
