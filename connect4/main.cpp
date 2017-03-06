// Steffen Lim Connect 4 algorithm
// g++ -std=c++11 -g -Wall -o assignment2 main.cpp -I./headers
#include "tree_node.h"
#include "grid.h"
#include "assesments.h"
#include "assesment.template"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace steffen_space;

// function prototypes
void get_user_settings(grid_state& grid);
void player_turn(grid_state& grid);
void oponent_turn(grid_state& grid);

// main
int main(int argc, char** argv){
    grid_state board;
    get_user_settings(board);
    while (1)
    {
        cout << board.to_string();
        player_turn(board);
        cout << board.to_string();
        oponent_turn(board);
    }
    
}

// function implementations
void get_user_settings(grid_state& grid){
    string board_width;
    string board_height;
    string connet_length;
    
    cout << "Width of board:";
    cin >> board_width;
    cout << "Height of board:";
    cin >> board_height;
    cout << "Connect length:";
    cin >> connet_length;

    grid = grid_state(stoi(board_width),stoi(board_height),stoi(connet_length));
}

void player_turn(grid_state& grid){
    string input;
    cout << "Player input(1-" << grid.get_grid().size() << "): ";
    cin >> input;
    size_t input_height = grid.set_data('X', stoi(input)-1);
    int result = utility<char>(grid.get_grid(), stoi(input)-1, (int)input_height, 'X');
    cout << "result: " << result << endl;
}

void oponent_turn(grid_state& grid){
    string input;
    cout << "Oponent input(1-" << grid.get_grid().size() << "): ";
    cin >> input;
    size_t input_height = grid.set_data('O', stoi(input)-1);
    int result = utility<char>(grid.get_grid(), stoi(input)-1, (int)input_height, 'O');
    cout << "result: " << result << endl;
}
