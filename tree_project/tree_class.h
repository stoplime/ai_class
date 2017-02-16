#ifndef _TREE_NODE_CLASS_H_
#define _TREE_NODE_CLASS_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include "node_class.h"

namespace steffen_space{

	template<class unit>
	class tree_node
	{
	private:
		/// the data unit
		unit data;
		/// parent pointer
		tree_node* parent;
		/// vector of children pointers
		vector<tree_node*> children;

	public:
		/// Constructor: empty 		-set as root where data is "ROOT"
		///				 overloaded -set parent and data
		tree_node();
		tree_node(tree_node* parent, const unit& init_data);
		/// Destructor: delete parent and children
		~tree_class();
		
		/// Getters and Setters functions:
		unit& get_data(){
			return data;
		}
		tree_node* get_parent(){
			return parent;
		}
		vector<tree_node*> get_children(){
			return children;
		}

		void set_data(const unit& new_data){
			data = new_data;
		}
		void set_parent(tree_node* new_parent){
			parent = new_parent;
		}
		void set_children(vector<tree_node*> new_children){
			children = new_children;
		}
		void append_child(tree_node* new_child){
			children.append(new_child);
		}
		void disown_child(std::size_t index){
			children.erase(std::remove(children.begin(), children.end(), index), children.end());
		}

		/// checks if leaf node
		bool is_leaf() const {
			return children.size()==0;
		}
	};

	/// Insert new child
	node_class* new_child(std::string _behavior, std::string _response);
	node_class* new_child(node_class &_parent, std::string _behavior, std::string _response);
	
	/// Depth first search
	node_class* depth_search(std::string search);
	node_class* depth_search(std::string search, node_class* current_node);

	/// Breadth first search
	node_class* breadth_search(std::string search);
	node_class* breadth_search(std::string search, std::vector<node_class*> current_node);
	
}
#endif
