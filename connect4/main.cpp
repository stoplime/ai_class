// Steffen Lim Connect 4 algorithm
// g++ -g -Wall -o assignment2 main.cpp -I./headers
#include "tree_node.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace steffen_space;

// function prototypes
void get_user_settings();

// main
int main(int argc, char** argv){
    get_user_settings();
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

    cout << "M: " << board_width << endl;
    cout << "N: " << board_height << endl;
    cout << "R: " << connet_length << endl;
}
