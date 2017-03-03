
#include "node_class.h"

namespace steffen_space{

    node_class::node_class(){
        behavior = "ROOT";
        response = "";
        is_bahavior = true;
    }
    node_class::node_class(std::string _behavior, std::string _response){
        behavior = _behavior;
        response = _response;
        if(_behavior == ""){
            is_bahavior = false;
        }
        else{
            is_bahavior = true;
        }
    }

    /// Getters and Setters for the behavior
    std::string node_class::get_behavior(){
        return behavior;
    }
    void node_class::set_behavior(std::string value){
        behavior = value;
        is_bahavior = true;
    }

    /// Getters and Setters for the response
    std::string node_class::get_response(){
        return response;
    }
    void node_class::set_response(std::string value){
        response = value;
        is_bahavior = false;
    }

    /// returns the string of a print function
    std::string node_class::to_string (){
        if (is_bahavior){
            return "behavior = " + behavior;
        }else{
            return "response = " + response;
        }
    }

}
