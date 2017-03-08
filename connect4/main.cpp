// Steffen Lim Connect 4 algorithm
// g++ -std=c++11 -g -Wall -o assignment2 main.cpp
#include "tree_node.h"
#include "assesments.h"
#include "grid.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace steffen_space;

assesment<> board;

// function prototypes
void get_user_settings();
void player_turn();
void oponent_turn();

// main
int main(int argc, char** argv){
    get_user_settings();
    while (1)
    {
        cout << board.current_grid.to_string();
        player_turn();
        cout << board.current_grid.to_string();
        oponent_turn();
    }
    
}

// function implementations
void get_user_settings(){
    string board_width;
    string board_height;
    string connet_length;
    
    cout << "Width of board:";
    cin >> board_width;
    cout << "Height of board:";
    cin >> board_height;
    cout << "Connect length:";
    cin >> connet_length;

    board = assesment<stoi(board_width),stoi(board_height),stoi(connet_length)>();
}

void player_turn(){
    string input;
    cout << "Player input(1-" << board.current_grid.get_grid().size() << "): ";
    cin >> input;
    size_t input_height = board.current_grid.set_data('X', stoi(input)-1);
    float result = utility<char>(board.current_grid.get_grid(), stoi(input)-1, (int)input_height, 'X');
    cout << "result: " << result << endl;
}

void oponent_turn(grid_state& grid){
    string input;
    cout << "Oponent input(1-" << board.current_grid.get_grid().size() << "): ";
    cin >> input;
    size_t input_height = board.current_grid.set_data('O', stoi(input)-1);
    float result = utility<char>(board.current_grid.get_grid(), stoi(input)-1, (int)input_height, 'O');
    cout << "result: " << result << endl;
}
