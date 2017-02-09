#include "xml_parse.h"

/// read file
tree_class* xml_parse_class::parse_into_tree(std::string file){
    std::ifstream stm(file.c_str());
    std::string str((std::istreambuf_iterator<char>(stm)), std::istreambuf_iterator<char>());
    std::cout << "str: " << str << std::endl;
    return new tree_class;
}

/// recursive parser
node_class* xml_parse_class::recursive_parse(){
    std::string tag;
    return new node_class;
}

/// parse arguments
void xml_parse_class::get_arguments(){

}

