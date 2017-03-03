// The node for holding the state space
#ifndef _STATE_SPACE_H_
#define _STATE_SPACE_H_
#include<vector>

namespace steffen_space{
    class state_node : public _node_class
    {
        //
    private:
        std::char data;

    public:
        /// Constructor:
        state_node();
        state_node(char input);

        /// Encapsulation
        char get_data(){
            return data;
        }
        void set_data(char value){
            if(value == 'O' || value == 'X' || value == '.'){
                data = value;
            }
        }
        void set_data(int value){
            switch(value){
                case 1:
                    data = 'O';
                    break;
                case 2:
                    data = 'X';
                    break;
                default:
                    data = '.';
                    break;
            }
        }
    }
}

#endif

