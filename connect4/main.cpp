// Steffen Lim Connect 4 algorithm
// g++ -std=c++11 -g -Wall -o assignment2 main.cpp
#include "tree_node.h"
#include "assesments.h"
#include "grid.h"
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>

using namespace std;
using namespace steffen_space;

// TODO: turn everything back into vector

// default
int width = 7;
int height = 6;
int connect = 4;
int ai_depth = 5;
int count_turns = 0;

grid_state pre_state;

// function prototypes
void get_user_settings();
bool player_turn(assesment& grid, char char_input, grid_state& prev);
bool ai_X_turn(assesment& grid);
bool ai_O_turn(assesment& grid);

// main
int main(int argc, char** argv){
    get_user_settings();
    assesment board(width, height, connect);
    board.set_max_depth(ai_depth);
    cout << board.get_max_depth() << endl;
    cout << board.get_grid_state().to_string();
    pre_state = board.get_grid_state();
    while (1)
    {
        // bool result = player_turn(board, 'X', pre_state); /*
        bool result = ai_X_turn(board); //*/
        cout << board.get_grid_state().to_string();
        if (result){
            cout << "X won!" << endl;
            return 0;
        }
        else if(++count_turns >= height*width){
            cout << "Its a tie!" << endl;
            return 0;
        }
        
        // board.heuristics(board.get_grid_state());
        // board.build_state_space();
        // cout << endl;
        
        /*
        result = player_turn(board, 'O');
        cout << board.get_grid_state().to_string();
        if (result >= 1000){
            cout << "O won!" << endl;
            return 0;
        }
        else if(++count_turns >= height*width){
            cout << "Its a tie!" << endl;
            return 0;
        }
        // board.heuristics(board.get_grid_state());
        board.build_state_space();
        //*/
        
        // result = ai_O_turn(board); /*
        result = player_turn(board, 'O', pre_state); //*/
        cout << board.get_grid_state().to_string();
        if (result){
            cout << "O won!" << endl;
            return 0;
        }
        else if(++count_turns >= height*width){
            cout << "Its a tie!" << endl;
            return 0;
        }
        //*/
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

bool player_turn(assesment& grid, char char_input, grid_state& prev){
    string input_str;
    cout << "Player input(0-" << grid.get_grid().size()-1 << "): ";
    cin >> input_str;
    int input = stoi(input_str);
    if(input == -1){
        grid.get_grid_state() = prev;
        cout << grid.get_grid_state().to_string();
        count_turns-=2;
        return player_turn(grid, char_input, prev);
    }
    else{
        prev = grid.get_grid_state();
    }

    int input_height = grid.get_grid_state().set_data(char_input, input);
    bool result = grid.utility(input, input_height);
    // cout << "result: " << result << endl;
    return result;
}

bool ai_X_turn(assesment& grid){
    clock_t start;
    double duration;
    start = clock();

    int input = grid.build_state_space('X');

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "duration: " << duration << " second(s)" << endl;
    cout << "AI X input: " << input << endl;
    int input_height = grid.get_grid_state().set_data('X', input);
    bool result = grid.utility(input, input_height);
    return result;
}

bool ai_O_turn(assesment& grid){
    clock_t start;
    double duration;
    start = clock();

    int input = grid.build_state_space('O');

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "duration: " << duration << " second(s)" << endl;
    cout << "AI O input: " << input << endl;
    int input_height = grid.get_grid_state().set_data('O', input);
    bool result = grid.utility(input, input_height);
    return result;
}
