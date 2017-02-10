#ifndef _XML_PARSE_CLASS_H_
#define _XML_PARSE_CLASS_H_

#include "node_class.h"
#include "tree_class.h"
#include <fstream>
#include <streambuf>
#include <regex>

class xml_parse_class
{
private:
	
public:
    /// read file
    tree_class* parse_into_tree(std::string file);

	/// recursive parser
    node_class* recursive_parse(std::string input);

    /// parse arguments
    void get_arguments(std::string tag, node_class* node);

    /// run a regex and return string vector
    vector<std::string> parse_regex(std::string reg_ex, std::string input);
};
#endif
