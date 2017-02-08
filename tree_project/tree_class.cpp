#include <iostream>
#include <vector>
#include "tree_class.h"

/// Constructor: create empty tree object
tree_class::tree_class()
{
	root = new node_class();
}
/// Destructor:
tree_class::~tree_class (){
	delete root;
}

node_class* tree_class::get_root(){
	return root;
}

/// Insert new child
node_class* tree_class::new_child(node_class &_parent, std::string _behavior, std::string _response){
	node_class* new_node = new node_class();
	new_node->set_behavior(_behavior);
	new_node->set_response(_response);
	new_node->set_parent(_parent);
	_parent.insert_child(*new_node);
	return new_node;
}
node_class* tree_class::new_child(std::string _behavior, std::string _response){
	return new_child(*root, _behavior, _response);
}

/// Depth first search
node_class* tree_class::depth_search(std::string search, node_class* current_node){
	if(current_node->get_behavior() == search){
		return current_node;
	}
	if(current_node->get_response() == search){
		return current_node;
	}
	int child_size = current_node->get_children().size();
	if(child_size > 0){
		node_class* return_node = NULL;
		int iter_child = 0;
		while (return_node == NULL){
			return_node = depth_search(search, current_node->get_children()[iter_child]);
			if(iter_child >= child_size-1){
				break;
			}
			iter_child++;
		}
		return return_node;
	}
	return NULL;
}
node_class* tree_class::depth_search(std::string search){
	return depth_search(search, root);
}

/*
/// Breadth first search
node_class tree_class::breadth_search(str::string search){
	
}

/// returns string from the print functions
std::string tree_class::get_print (){
	
}*/
