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
    short int utility(vector< vector<type> > grid, int x, int y, type turn){
        assert(grid.size() > 0);
        // check vertical
        type prev_value = '';
        int count = 0;
        for (size_t i = 0; i < grid[x].size(); i++)
        {
            type value = grid[x][i];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(++count >= connect_length && value != empty_value){
                    if(value == turn){
                        return 1000;
                    }
                    else{
                        return -1000;
                    }
                }
            }
        }

        // check horizontal
        prev_value = '';
        count = 0;
        for (size_t i = 0; i < grid.size(); i++)
        {
            type value = grid[i][y];
            if(value != prev_value){
                count = 1;
                prev_value = value;
            }
            else{
                if(++count >= connect_length && value != empty_value){
                    if(value == turn){
                        return 1000;
                    }
                    else{
                        return -1000;
                    }
                }
            }
        }

        // check diagonals
        // start from bottom-left -- row side
        for(size_t i=0; i <= grid.size() - connect_length; ++i){
            count = 0;
            prev_value = '';
            int row = i;
            int col = 0;
            while(row < grid.size() && col < grid[row].size()){
                type value = grid[row][col].get_data();
                if(value != prev_value){
                    count = 1;
                    prev_value = value;
                }
                else{
                    if(++count >= connect_length && value != empty_value){
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
        // start from bottom-left -- col side
        for(size_t i=1; i <= grid[x].size() - connect_length; ++i){
            count = 0;
            prev_value = '';
            int row = 0;
            int col = i;
            while(row < grid[x].size() && col < grid.size()){
                type value = grid[row][col].get_data();
                if(value != prev_value){
                    count = 1;
                    prev_value = value;
                }
                else{
                    if(++count >= connect_length && value != empty_value){
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
        // start from bottom-right -- row side
        for(size_t i=grid.size()-1; i >= connect_length-1; --i){
            count = 0;
            prev_value = '';
            int row = i;
            int col = 0;
            while(row >= 0 && col < grid[row].size()){
                type value = grid[row][col].get_data();
                if(value != prev_value){
                    count = 1;
                    prev_value = value;
                }
                else{
                    if(++count >= connect_length && value != empty_value){
                        if(value == turn){
                            return 1000;
                        }
                        else{
                            return -1000;
                        }
                    }
                }
                --row;
                ++col;
            }
        }
        // start from bottom-right -- col side
        for(size_t i=1; i <= grid[x].size() - connect_length; ++i){
            count = 0;
            prev_value = '';
            int row = grid.size();
            int col = i;
            while(row >= 0 && col < grid.size()){
                type value = grid[row][col].get_data();
                if(value != prev_value){
                    count = 1;
                    prev_value = value;
                }
                else{
                    if(++count >= connect_length && value != empty_value){
                        if(value == turn){
                            return 1000;
                        }
                        else{
                            return -1000;
                        }
                    }
                }
                --row;
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
