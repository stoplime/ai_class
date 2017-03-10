// The node for holding the state space
#ifndef _STATE_SPACE_H_
#define _STATE_SPACE_H_
#include <array>
#include <algorithm>
#include <string>

namespace steffen_space{

    template<int width = 7, int height = 6>
    class grid_state
    {
    private:
        std::array<std::array<char, height>, width> grid;
        float score;
        int input;
    public:
        grid_state();
        grid_state(float score, int input);
        ~grid_state();
        grid_state<width, height> copy();

        /// Encapsulation
        std::array<std::array<char, height>, width> get_grid();

        size_t set_data(char value, size_t x);
        size_t set_data(int value, size_t x);

        float get_score();
        void set_score(float value);

        int get_input();
        void set_input(int value);

        int get_height_empty(int x);

        std::string to_string();
    };
}

#include "grid.template"
#endif

