//g++ -g -Wall -o assignment1 main.cpp tree_class.cpp node_class.cpp

#include "tree_class.h"
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
	cout << "test" << endl;
	return 0;
}
