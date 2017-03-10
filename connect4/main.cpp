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
template<class assesment_type>
float player_turn(assesment_type grid);
template<class assesment_type>
float oponent_turn(assesment_type grid);

// main
int main(int argc, char** argv){
    get_user_settings();
    assesment<width, height, connect> board(grid_state<width, height>(), 'O');
    while (1)
    {
        cout << board.get_grid().to_string();
        float result = player_turn<assesment<width, height, connect>>(board);
        if (result >= 1000){
            return;
        }
        cout << board.get_grid().to_string();
        result = oponent_turn<board_type>(board);
        if (result >= 1000){
            return;
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

template<class assesment_type>
float player_turn(assesment_type& grid){
    string input;
    cout << "Player input(1-" << grid.current_grid.get_grid().size() << "): ";
    cin >> input;
    size_t input_height = grid.current_grid.set_data('X', stoi(input)-1);
    float result = grid.utility(stoi(input)-1, (int)input_height);
    cout << "result: " << result << endl;
    return result;
}

template<class assesment_type>
float oponent_turn(assesment_type& grid){
    string input;
    cout << "Oponent input(1-" << grid.current_grid.get_grid().size() << "): ";
    cin >> input;
    size_t input_height = grid.current_grid.set_data('O', stoi(input)-1);
    float result = grid.utility(stoi(input)-1, (int)input_height);
    cout << "result: " << result << endl;
}
