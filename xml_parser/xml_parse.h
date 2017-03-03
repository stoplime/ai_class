#ifndef _XML_PARSE_CLASS_H_
#define _XML_PARSE_CLASS_H_

#include "node_class.h"
#include <fstream>
#include <streambuf>
#include <regex>
#include <queue>    

namespace steffen_space
{
    /// read file
    tree_node<node_class>* parse_into_tree(std::string file);

    /// recursive call to iterate through the tree and the queue of tags
    void tag_parse(tree_node<node_class>* branch, std::queue<std::string>& tags);

    /// gets the behavior and response values out of a tag string
    node_class get_node(const std::string tag);
}

#endif
