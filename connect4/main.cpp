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

// TODO: turn everything back into vector

// default
int width = 7;
int height = 6;
int connect = 4;

// function prototypes
void get_user_settings();
float player_turn(assesment& grid);
float oponent_turn(assesment& grid);

// main
int main(int argc, char** argv){
    get_user_settings();
    assesment board(width, height, connect, 'O');
    while (1)
    {
        cout << board.get_grid_state().to_string();
        float result = player_turn(board);
        if (result >= 1000){
            return 0;
        }
        cout << board.get_grid_state().to_string();
        result = oponent_turn(board);
        if (result >= 1000){
            return 0;
        }
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

    width = stoi(board_width);
    height = stoi(board_height);
    connect = stoi(connet_length);
}

float player_turn(assesment& grid){
    string input_str;
    cout << "Player input(1-" << grid.get_grid().size() << "): ";
    cin >> input_str;
    int input = stoi(input_str)-1;
    int input_height = grid.get_grid_state().set_data('X', input);
    float result = grid.utility(input, input_height);
    cout << "result: " << result << endl;
    return result;
}

float oponent_turn(assesment& grid){
    int input = grid.build_state_space();
    int input_height = grid.get_grid_state().set_data('O', input);
    float result = grid.utility(input, input_height);
    return result;
}
