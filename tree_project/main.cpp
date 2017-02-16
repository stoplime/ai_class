//g++ -g -Wall -o assignment1 main.cpp xml_parse.cpp tree_class.cpp node_class.cpp

#include "tree_class.h"
#include "xml_parse.h"
#include <cassert>

using namespace std;

int main()
{
	tree_class t;
	t.new_child("Idle", "");
	t.new_child("Incoming Projectile", "");
	t.new_child("Combat", "");
	t.new_child(*t.breadth_search("Idle"), "", "Use Computer");
	t.new_child(*t.breadth_search("Idle"), "", "Patrol");
	t.new_child(*t.breadth_search("Incoming Projectile"), "", "Evade");
	t.new_child(*t.breadth_search("Combat"), "Melee", "");
	t.new_child(*t.breadth_search("Combat"), "Ranged", "");
	t.new_child(*t.breadth_search("Melee"), "", "Flee");
	t.new_child(*t.breadth_search("Melee"), "", "Attack");
	t.new_child(*t.breadth_search("Ranged"), "", "Weapon 1");
	t.new_child(*t.breadth_search("Ranged"), "", "Weapon 2");
	t.new_child(*t.breadth_search("Ranged"), "", "Weapon 3");
	
	cout << t.breadth_search("Idle")->get_print() << endl;
	xml_parse_class x;
	x.parse_into_tree("/home/stoplime/ai_class/tree_project/test.xml");
	return 0;
}
