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
	cout << t.depth_search("something2")->get_print() << endl;
	cout << "test" << endl;
	return 0;
}
