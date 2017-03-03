#ifndef _NODE_CLASS_H_
#define _NODE_CLASS_H_

#include <string>
#include <vector>
#include "tree_node.h"

namespace steffen_space{
	class node_class : public _node_class
	{
	private:
		std::string behavior;
		std::string response;
		bool is_bahavior;

	public:
		/// Constructor: initializes private vars
		node_class();
		node_class(std::string _behavior, std::string _response);

		/// Getters and Setters for the behavior
		std::string get_behavior();
		void set_behavior(std::string value);

		/// Getters and Setters for the response
		std::string get_response();
		void set_response(std::string value);

		/// returns the string of a print function
		std::string to_string ();
	};
}
#endif
