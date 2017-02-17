#ifndef _TREE_NODE_CLASS_H_
#define _TREE_NODE_CLASS_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include "node_class.h"

namespace steffen_space
{
	template<class unit>
	class tree_node
	{
	private:
		/// the node_class => unit data
		unit data;
		/// vector of children pointers
		std::vector< std::shared_ptr<tree_node*> > children;

	public:
		/// Constructor: empty 		-set as root where data is "ROOT"
		///				 overloaded -set data
		///				 overloaded -set data and children
		tree_node();
		tree_node(const unit& init_data);
		tree_node(const unit& init_data, std::vector<tree_node*> _children);
		
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
		void set_parent(tree_node* new_parent){
			parent = new_parent;
		}
		void set_children(std::vector<tree_node*> new_children){
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
	/*
	/// Insert new child
	node_class* new_child(std::string _behavior, std::string _response);
	node_class* new_child(node_class &_parent, std::string _behavior, std::string _response);
	*/

	/// Depth first search (preorder)
	template <class process, class node_class>
	void depth_search(process f, node_class* node);

	/// Breadth first search
	template <class process, class node_class>
	void breadth_search(process f, node_class* node);

	/// Print tree
	template <class unit, class size_type>
    void print(binary_tree_node<Item>* node, size_type depth);
	template <class unit>
    void print(binary_tree_node<Item>* node);

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
		virtual std::string get_print(){
			return "";
		}
	}

}
#include "tree_node.template"
#endif
