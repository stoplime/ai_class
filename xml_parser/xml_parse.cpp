#include "xml_parse.h"

// regex extracting  tags:  <[^>]*
// regex extracting of / :  \\/
// regex quote extraction:  (["'])(?:(?=(\\?))\2.)*?\1
// extraction with escape:  ([\"\'])(?:(?=(\\\\?))\\2.)*?\\1
// extraction of behavior:  behavior\\s?=\\s?([\"\'])(?:(?=(\\\\?))\\2.)*?\\1
// extraction of response:  response\\s?=\\s?([\"\'])(?:(?=(\\\\?))\\2.)*?\\1

namespace steffen_space
{
    /// read file
    tree_node<node_class>* parse_into_tree(std::string file){
        std::ifstream stm(file.c_str());
        std::string str((std::istreambuf_iterator<char>(stm)), std::istreambuf_iterator<char>());
        //std::cout << "str: " << str << std::endl;
        std::queue<std::string> tags;
        std::regex tag_regex("<[^>]*");
        for (std::sregex_iterator i = std::sregex_iterator(str.begin(), str.end(), tag_regex); i != std::sregex_iterator(); i++){
            std::smatch tag_str = *i;
            std::string tag_string = tag_str.str();
            tag_string.erase(std::remove(tag_string.begin(), tag_string.end(), '<'), tag_string.end());
            //std::cout << "tag: " << tag_string << std::endl;
            tags.push(tag_string);
        }
        if(tags.front() != "root"){
            // tree does not start with root
            // well not sure what to do here
            std::cout << "error: tree did not start with root" << std::endl;
        }
        tags.pop();
        tree_node<node_class>* t0 = new tree_node<node_class> ();
        tag_parse(t0, tags);
        return t0;
    }
    
    /// recursive parser
    void tag_parse(tree_node<node_class>* branch, std::queue<std::string>& tags){
        // check if there is a /
        std::size_t end_pos = tags.front().find('/');
        if(end_pos == std::string::npos || end_pos != 0){
            //there is not a / or the / is at the end
            // make a child object and append it to the branch
            node_class new_node = get_node(tags.front());
            tree_node<node_class>* new_branch = new tree_node<node_class> (new_node);
            branch->append_child(new_branch);
            //std::cout << "new_branch: " << tags.front() << std::endl;
            tags.pop();
            // then recurse to do all the children of new_branch
            if(end_pos != std::string::npos){
                return;
            }
            // run through all children until the tag has a / at the beginning
            while(!tags.empty() && (tags.front().find('/') == std::string::npos || tags.front().find('/') != 0)){
                tag_parse(new_branch, tags);
            }
            // after the while loop, there is a / and it's at the beginning
            tags.pop();
        }
        // if there is a / and it's at the beginning
        else{
            tags.pop();
            return;
        }

        // run through the next child under current branch until tag has a / at the beginning
        while(!tags.empty() && (tags.front().find('/') == std::string::npos || tags.front().find('/') != 0)){
            tag_parse(branch, tags);
        }
    }

    node_class get_node(const std::string tag){
        // extract the behavior and response arguments
        std::regex behavior_regex("behavior\\s?=\\s?([\"\'])(?:(?=(\\\\?))\\2.)*?\\1");
        std::regex response_regex("response\\s?=\\s?([\"\'])(?:(?=(\\\\?))\\2.)*?\\1");
        std::string behavior_value;
        std::string response_value;

        for (std::sregex_iterator i = std::sregex_iterator(tag.begin(), tag.end(), behavior_regex); i != std::sregex_iterator(); i++){
            std::smatch behavior_match = *i;
            std::string behavior_str = behavior_match.str();
            
            std::string::size_type start_pos = 0;
            std::string::size_type end_pos = 0;

            start_pos = behavior_str.find("\"");
            if(start_pos != std::string::npos){
                // start after the "
                start_pos++;
                end_pos = behavior_str.find("\"", start_pos);
                if(end_pos != std::string::npos){
                    behavior_value = behavior_str.substr(start_pos, end_pos-start_pos);
                }
            }
        }
        for (std::sregex_iterator i = std::sregex_iterator(tag.begin(), tag.end(), response_regex); i != std::sregex_iterator(); i++){
            std::smatch response_match = *i;
            std::string response_str = response_match.str();
            
            std::string::size_type start_pos = 0;
            std::string::size_type end_pos = 0;

            start_pos = response_str.find("\"");
            if(start_pos != std::string::npos){
                // start after the "
                start_pos++;
                end_pos = response_str.find("\"", start_pos);
                if(end_pos != std::string::npos){
                    response_value = response_str.substr(start_pos, end_pos-start_pos);
                }
            }
        }
        node_class* _node = new node_class(behavior_value, response_value);
        return *_node;
    }
}
