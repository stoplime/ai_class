#ifndef _NODE_CLASS_H_
#define _NODE_CLASS_H_

#include <string>
#include <vector>

class node_class
{
private:
	/// Parent node
	node_class* parent;
	/// Vector of child nodes
	std::vector<node_class*> children;
	
	/// Stores the behavior string
	std::string behavior;
	/// Stores the response string
	std::string response;
public:
	/// Getters and Setters for the behavior
	std::string get_behavior();
	void set_behavior(std::string value);
	
	/// Getters and Setters for the response
	std::string get_response();
	void set_response(std::string value);

	/// Getter for children
	std::vector<node_class*> get_children();
	
	/// Constructor: initializes private vars
	node_class();
	
	/// Deconstructor:
	~node_class();
	
	///	initializes the parent node to _parent
	void set_parent(node_class &_parent);
	
	/// Insert child
	void insert_child(node_class &child);

	/// create new child node
	node_class* new_child(node_class &_parent, std::string _behavior, std::string _response);
	
	/// returns the string of a print function
	std::string get_print ();
};
#endif
