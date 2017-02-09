//g++ -g -Wall -o assignment1 main.cpp tree_class.cpp node_class.cpp

#include "tree_class.h"
#include "xml_parse.h"
#include <cassert>

using namespace std;

int main()
{
	tree_class t;
	t.new_child("some behavior", "");
	t.new_child("something", "");
	t.new_child("", "something2");
	t.new_child(*t.depth_search("something"), "", "child of something");
	cout << t.depth_search("child of something")->get_print() << endl;
	xml_parse_class x;
	x.parse_into_tree("/home/stoplime/ai_class/tree_project/test.xml");
	return 0;
}
