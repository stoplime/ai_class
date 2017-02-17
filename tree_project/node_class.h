#ifndef _NODE_CLASS_H_
#define _NODE_CLASS_H_

#include <string>
#include <vector>
#include "tree_node.h"

namespace steffen_space{
	class node_class : public _node_class
	{
	private:
		std::string bahavior;
		std::string response;
		bool is_bahavior;

	public:
		/// Getters and Setters for the behavior
		std::string get_behavior(){
			return behavior;
		}
		void set_behavior(std::string value){
			behavior = value;
		}

		/// Getters and Setters for the response
		std::string get_response(){
			return response;
		}
		void set_response(std::string value){
			response = value;
		}
		
		/// Constructor: initializes private vars
		node_class(){
			behavior = "ROOT";
			response = "";
			is_bahavior = true;
		}
		node_class(std::string _behavior, std::string _response){
			behavior = _behavior;
			response = _response;
			if(_behavior == ""){
				is_bahavior = false;
			}
			else{
				is_bahavior = true;
			}
		}

		/// returns the string of a print function
		std::string get_print (){
			if (is_bahavior){
				return "behavior = " + behavior;
			}else{
				return "response = " + response;
			}
		}
	};
}
#endif
