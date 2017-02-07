#include "node_class.h"

/// Getters and Setters for the behavior
std::string node_class::get_behavior(){
	return behavior;
}
void node_class::set_behavior(std::string value){
	behavior = value;
}

/// Getters and Setters for the response
std::string node_class::get_response(){
	return response;
}
void node_class::set_response(std::string value){
	response = value;
}

/// Getter for children
std::vector<node_class*> node_class::get_children(){
	return children;
}

/// Constructor: initializes private vars
node_class::node_class(){
	behavior = "";
	response = "";
	parent = NULL;
}

/// Deconstructor:
node_class::~node_class(){
	delete parent;
	children.clear();
}

///	initializes the parent node to _parent
void node_class::set_parent(node_class &_parent){
	parent = &_parent;
}

/// Insert child
void node_class::insert_child(node_class &child){
	children.push_back(&child);
}

/// returns the string of a print function
std::string node_class::get_print (){
	if (behavior != ""){
		return "behavior = " + behavior;
	}else{
		return "response = " + response;
	}
}
