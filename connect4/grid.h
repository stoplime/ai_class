// The node for holding the state space
#ifndef _GRID_NODE_H_
#define _GRID_NODE_H_
#include <vector>
#include <algorithm>
#include <string>

namespace steffen_space{
    class grid_state
    {
    private:
        std::vector< std::vector<char> > grid;
        std::vector<int> grid_heights;
        int width;
        int height;
        float score;
        int input;
    public:
        grid_state();
        grid_state(int width, int height);
        grid_state(int width, int height, float score, int input);
        grid_state(const grid_state& gt);
        //~grid_state();

        /// Encapsulation
        std::vector< std::vector<char> >& get_grid();

        int set_data(char value, int x);
        int set_data(int value, int x);

        float get_score();
        void set_score(float value);

        int get_input();
        void set_input(int value);

        int get_height_empty(int x);

        std::string to_string();
    };
}

#endif

