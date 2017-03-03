// Steffen Lim Connect 4 algorithm
// g++ -g -Wall -o assignment2 main.cpp -I./headers
#include "tree_node.h"
#include "grid.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace steffen_space;

// function prototypes
void get_user_settings(grid_state grid);

// main
int main(int argc, char** argv){
    grid_state board;
    get_user_settings(board);

}

// function implementations
void get_user_settings(grid_state grid){
    string board_width;
    string board_height;
    string connet_length;
    
    cout << "Width of board:";
    cin >> board_width;
    cout << "Height of board:";
    cin >> board_height;
    cout << "Connect length:";
    cin >> connet_length;

    grid
}
