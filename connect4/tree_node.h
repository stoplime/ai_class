// The tree data structure
#ifndef _TREE_NODE_CLASS_H_
#define _TREE_NODE_CLASS_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

namespace steffen_space
{
	template<class unit>
	class tree_node
	{
	private:
		/// the node_class => unit data
		unit data;
		/// vector of children pointers
		std::vector<tree_node*> children;

	public:
		/// Constructor: empty 		-set as root where data is "ROOT"
		///				 overloaded -set data
		///				 overloaded -set data and children
		tree_node();
		tree_node(const unit& init_data);
		tree_node(const unit& init_data, std::vector<tree_node*> _children);	

		~tree_node();
		
		/// Getters and Setters functions:
		unit& get_data(){
			return data;
		}
		std::vector<tree_node*> get_children(){
			return children;
		}
		tree_node* get_child(size_t i){
			return children[i];
		}
		tree_node* get_child(int i){
			return children[i];
		}
		size_t get_num_children(){
			return children.size();
		}

		void set_data(const unit& new_data){
			data = new_data;
		}
		void append_child(tree_node* new_child){
			children.push_back(new_child);
		}
		void disown_child(std::size_t index){
			children.erase(std::remove(children.begin(), children.end(), index), children.end());
		}

		/// checks if leaf node
		bool is_leaf() const {
			return children.size()==0;
		}
	};

	/// Depth first search using stacks
	template <typename type>
	type* depth_search(std::string search, type* node, int& count);

	/// Depth first search recursive
	template <typename type>
	type* depth_search_recursive(std::string search, type* node, int& count);

	/// Breadth first search using queues
	template <typename type>
	type* breadth_search(std::string search, type* node, int& count);

	/// Search function
	/// returns True when found

	/// Print tree
	template <class unit, class size_type>
    void print(tree_node<unit>* node, size_type depth);
	template <class unit>
    void print(tree_node<unit>* node);

	/****************************************************************************
	* tree_node class requires a node_class with specific functions				*
	* to ensure the node_class contains the required functions, a				*
	* template class wll be created to be inherited when defining				*
	* a node_class implementation.												*
	*****************************************************************************
	//*/

	class _node_class
	{
	public:
		virtual std::string to_string(){
			return "";
		}
	};

}

#include "tree_node.template"
#endif
