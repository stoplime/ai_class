// The node for holding the state space
#include "grid.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

namespace steffen_space{
    
    grid_state::grid_state()
    : grid_state(7, 6, 0, -1)
    {}

    grid_state::grid_state(int width, int height)
    : grid_state(width, height, 0, -1)
    {}
    
    grid_state::grid_state(int width, int height, float score, int input)
    : grid(width, std::vector<char>(height,'.')), grid_heights(width, 0)
    {
        this->width = width;
        this->height = height;
        this->score = score;
        this->input = input;
    }

    grid_state::grid_state(const grid_state& gt)
    : grid(gt.grid), grid_heights(gt.grid_heights)
    {
        this->width = gt.width;
        this->height = gt.height;
        this->score = gt.score;
        this->input = gt.input;
    }
    
    // grid_state grid_state::copy(){
    //     grid_state new_grid_state(width, height, score, input);
    //     new_grid_state.grid = grid;
    //     return new_grid_state;
    // }

    std::vector< std::vector<char> >& grid_state::get_grid(){
        return grid;
    }

    int grid_state::set_data(char value, int x){
        int input_value = 0;
        if(value == 'O'){
            input_value = 1;
        }
        else if(value == 'X'){
            input_value = 2;
        }
        return set_data(input_value, x);
    }

    int grid_state::set_data(int value, int x){
        if(x < width && x >= 0){
            int y = this->get_height_empty(x);
            if(y != -1){
                switch(value){
                    case 1:
                        grid[x][y] = 'O';
                        break;
                    case 2:
                        grid[x][y] = 'X';
                        break;
                    default:
                        grid[x][y] = '.';
                        break;
                }
                this->input = x;
                grid_heights[x]++;
            }
            return y;
        }
        return -1;
    }

    float grid_state::get_score(){
        return score;
    }
    
    void grid_state::set_score(float value){
        score = value;
    }

    int grid_state::get_input(){
        return input;
    }
    
    void grid_state::set_input(int value){
        input = value;
    }

    int grid_state::get_height_empty(int x){
        int y = -1;
        if(grid_heights[x] < height){
            y = grid_heights[x];
        }
        return y;
    }

    std::string grid_state::to_string(){
        std::string output = "";
        for (int j = height-1; j >= 0; --j)
        {
            for (int i = 0; i < width; ++i)
            {
                output += grid[i][j];
                output += " ";
            }
            output += "\n";
        }
        return output;
    }
}

