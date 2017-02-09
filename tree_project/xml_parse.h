#ifndef _XML_PARSE_CLASS_H_
#define _XML_PARSE_CLASS_H_

#include "node_class.h"
#include "tree_class.h"
#include <fstream>
#include <streambuf>

class xml_parse_class
{
private:
	
public:
    /// read file
    tree_class* parse_into_tree(std::string file);

	/// recursive parser
    node_class* recursive_parse();

    /// parse arguments
    void get_arguments();

};
#endif
