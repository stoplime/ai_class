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
	new_node->new_child(_parent, _behavior, _response);
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

/// Breadth first search
node_class* tree_class::breadth_search(std::string search, std::vector<node_class*> current_layer){
	std::vector<node_class*> next_layer;
	for(unsigned int i=0; i < current_layer.size(); i++){
		// if current node is the target, return it else append the children to next_layer
		if(current_layer[i]->get_behavior() == search){
			return current_layer[i];
		}
		if(current_layer[i]->get_response() == search){
			return current_layer[i];
		}
		for(unsigned int j=0; j < current_layer[i]->get_children().size(); j++){
			next_layer.push_back(current_layer[i]->get_children()[j]);
		}
	}
	// if next_layer == empty then search not exist
	if(next_layer.size() == 0){
		return NULL;
	}
	return breadth_search(search, next_layer);
}
node_class* tree_class::breadth_search(std::string search){
	std::vector<node_class*> vect_root;
	vect_root.push_back(root);
	return breadth_search(search, vect_root);
}

/*
/// returns string from the print functions
std::string tree_class::get_print (){
	
}*/
