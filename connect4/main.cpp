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
int ai_depth = 5;

// function prototypes
void get_user_settings();
float player_turn(assesment& grid, char char_input);
float ai_2_turn(assesment& grid);
float oponent_turn(assesment& grid);

// main
int main(int argc, char** argv){
    get_user_settings();
    assesment board(width, height, connect, 'O');
    board.set_max_depth(ai_depth);
    cout << board.get_max_depth() << endl;
    cout << board.get_grid_state().to_string();
    int count_turns = 0;
    while (1)
    {
        float result = player_turn(board, 'X');
        cout << board.get_grid_state().to_string();
        if (result >= 1000){
            cout << "X won!" << endl;
            return 0;
        }
        else if(++count_turns >= height*width){
            cout << "Its a tie!" << endl;
            return 0;
        }
        // board.heuristics(board.get_grid_state());
        cout << endl;
        /*
        result = player_turn(board, 'O');
        cout << board.get_grid_state().to_string();
        if (result >= 1000){
            cout << "X won!" << endl;
            return 0;
        }
        else if(++count_turns >= height*width){
            cout << "Its a tie!" << endl;
            return 0;
        }
        board.heuristics(board.get_grid_state());
        //*/
        result = oponent_turn(board);
        cout << board.get_grid_state().to_string();
        if (result >= 1000){
            cout << "O won!" << endl;
            return 0;
        }
        else if(++count_turns >= height*width){
            cout << "Its a tie!" << endl;
            return 0;
        }
    }
    
}

// function implementations
void get_user_settings(){
    string board_width;
    string board_height;
    string connet_length;
    string ai_depth_str;
    
    cout << "Width of board:";
    cin >> board_width;
    cout << "Height of board:";
    cin >> board_height;
    cout << "Connect length:";
    cin >> connet_length;
    cout << "AI depth:";
    cin >> ai_depth_str;

    width = stoi(board_width);
    height = stoi(board_height);
    connect = stoi(connet_length);
    ai_depth = stoi(ai_depth_str);
}

float player_turn(assesment& grid, char char_input){
    string input_str;
    cout << "Player input(1-" << grid.get_grid().size() << "): ";
    cin >> input_str;
    int input = stoi(input_str)-1;
    int input_height = grid.get_grid_state().set_data(char_input, input);
    float result = grid.utility(input, input_height);
    cout << "result: " << result << endl;
    return result;
}

float ai_2_turn(assesment& grid){
    int input = grid.build_state_space();
    cout << "AI X input: " << input << endl;
    int input_height = grid.get_grid_state().set_data('X', input);
    float result = grid.utility(input, input_height);
    return result;
}

float oponent_turn(assesment& grid){
    int input = grid.build_state_space();
    cout << "AI O input: " << input << endl;
    int input_height = grid.get_grid_state().set_data('O', input);
    float result = grid.utility(input, input_height);
    return result;
}
