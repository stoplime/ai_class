#ifndef _TREE_CLASS_H_
#define _TREE_CLASS_H_

#include <iostream>
#include <vector>
#include "node_class.h"

class tree_class
{
private:
	/// the root tree node
	node_class* root;
public:
	/// Constructor: create empty tree object
	tree_class ();
	/// Destructor:
	~tree_class ();
	
	node_class* get_root();
	
	/// Insert new child
	void new_child(std::string _behavior, std::string _response);
	void new_child(node_class &_parent, std::string _behavior, std::string _response);
	
	/// Depth first search
	node_class* depth_search(std::string search);
	node_class* depth_search(std::string search, node_class* current_node);

	/*
	/// Breadth first search
	node_class breadth_search(std::string search);
	
	/// returns string from the print functions
	std::string get_print ();
	*/
};
#endif
