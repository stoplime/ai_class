// The node for holding the state space
#ifndef _STATE_SPACE_H_
#define _STATE_SPACE_H_
#include <vector>
#include <string>

namespace steffen_space{
    class grid_state
    {
        //
    private:
        std::vector< std::vector<char> > grid;

    public:
        /// Constructor:
        grid_state(int width, int height, int connect){
            for (int i = 0; i < width; ++i)
            {
                grid.push_back(std::vector<char>(height,'.'));
            }
        }
        grid_state()
        : grid_state(7, 6, 4) {}
        
        /// Encapsulation
        std::vector< std::vector<char> > get_grid(){
            return grid;
        }
        size_t set_data(char value, size_t x){
            int input = 0;
            if(value == 'O'){
                input = 1;
            }
            else if(value == 'X'){
                input = 2;
            }
            return set_data(input, x);
        }
        size_t set_data(int value, size_t x){
            if(x < grid.size()){
                size_t y = 0;
                for (size_t i = 0; i < grid[x].size(); i++)
                {
                    if(grid[x][i] == '.'){
                        y = i;
                        break;
                    }
                }
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
                return y;
            }
            return -1;
        }

        std::string to_string(){
            std::string output = "";
            for (int j = grid[0].size()-1; j >= 0; --j)
            {
                for (size_t i = 0; i < grid.size(); ++i)
                {
                    output += grid[i][j];
                }
                output += "\n";
            }
            return output;
        }
    };
}

#endif

