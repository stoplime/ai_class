// The tree data structure
#ifndef _STATE_SPACE_CLASS_H_
#define _STATE_SPACE_CLASS_H_
#include <array>
#include <vector>

namespace steffen_space
{
    template<int width = 7, int height = 6>
    class state_space
    {
    public:
        state_space(std::array<std::array<char, height>, width> grid);
        ~state_space();
        copy(std::array<std::array<char, height>, width> grid);
    private:
        std::vector<*char> space;
    };

    template<int width, int height>
    state_space::state_space(std::array<std::array<char, height>, width> grid){
        
    }

    template<int width, int height>
    state_space::copy(std::array<std::array<char, height>, width> grid){
        
    }
}
#endif