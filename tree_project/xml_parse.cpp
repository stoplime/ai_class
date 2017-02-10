#include "xml_parse.h"

// regex extracting  tags:  <.+(?=>)
// regex quote extraction:  (["'])(?:(?=(\\?))\2.)*?\1
// extraction with escape:  ([\"\'])(?:(?=(\\\\?))\\2.)*?\\1
// extraction of behavior:  behavior=(["'])(?:(?=(\\?))\2.)*?\1
// extraction of response:  response=(["'])(?:(?=(\\?))\2.)*?\1


/// read file
tree_class* xml_parse_class::parse_into_tree(std::string file){
    std::ifstream stm(file.c_str());
    std::string str((std::istreambuf_iterator<char>(stm)), std::istreambuf_iterator<char>());
    //std::cout << "str: " << str << std::endl;

    std::regex tag_regex("<.+(?=>)");
    for (std::sregex_iterator i = std::sregex_iterator(str.begin(), str.end(), tag_regex); i != std::sregex_iterator(); ++i){
        std::smatch tag_str = *i;
        std::string tag_string = tag_str.str();
        tag_string.erase(std::remove(tag_string.begin(), tag_string.end(), '<'), tag_string.end());
        
    }
    return new tree_class;
}

/// recursive parser
node_class* xml_parse_class::recursive_parse(std::string input){
    std::string tag;
    return new node_class;
}

/// parse arguments
void xml_parse_class::get_arguments(std::string tag, node_class* node){
    std::regex get_behavior("behavior=([\"\'])(?:(?=(\\\\?))\\2.)*?\\1");
    //std::regex get_response(R"(response=(["'])(?:(?=(\\?))\2.)*?\1)");
    
    //std::regex get_quote(R"((["'])(?:(?=(\\?))\2.)*?\1)");

    std::smatch behavior_str;
    std::regex_search(tag, behavior_str, get_behavior);
    //std::cout << str(behavior_str) << std::endl;
}

/// run a regex and return string vector
vector<std::string> xml_parse_class::parse_regex(std::string reg_ex, std::string input){

}

