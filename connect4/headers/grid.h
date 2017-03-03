// The node for holding the state space
#ifndef _STATE_SPACE_H_
#define _STATE_SPACE_H_
#include<vector>

namespace steffen_space{
    class grid_state
    {
        //
    private:
        std::vector< std::vector<std::char> > grid;

    public:
        /// Constructor:
        grid_state(int width, int height, int connect){
            for (size_t i = 0; i < width; ++i)
            {
                grid.push_back(std::vector<std::char>(height,'.'));
            }
        }
        grid_state()
        : grid_state(7, 6, 4) {}
        
        /// Encapsulation
        char get_grid(){
            return data;
        }
        void set_data(char value, int x, int y){
            if(x < grid.size() && y < grid[0].size()){
                if(value == 'O' || value == 'X' || value == '.'){
                    grid[x][y] = value;
                }
            }
        }
        void set_data(int value, int x, int y){
            if(x < grid.size() && y < grid[0].size()){
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
            }
        }
    }
}

#endif

