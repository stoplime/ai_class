//g++ -g -Wall -o assignment1 main.cpp xml_parse.cpp tree_class.cpp node_class.cpp

#include "node_class.h"
#include "xml_parse.h"
#include <cassert>

using namespace std;
using namespace steffen_space;

int main()
{
	tree_node<node_class>* t1 = new tree_node<node_class> ();
	tree_node<node_class>* t0 = new tree_node<node_class> ();

	t.append_child(node_class("Idle", ""));
	t.append_child(node_class("Incoming Projectile", ""));
	t.append_child(node_class("Combat", ""));
	t.append_child(*t.breadth_search("Idle"), "", "Use Computer");
	t.append_child(*t.breadth_search("Idle"), "", "Patrol");
	t.append_child(*t.breadth_search("Incoming Projectile"), "", "Evade");
	t.append_child(*t.breadth_search("Combat"), "Melee", "");
	t.append_child(*t.breadth_search("Combat"), "Ranged", "");
	t.append_child(*t.breadth_search("Melee"), "", "Flee");
	t.append_child(*t.breadth_search("Melee"), "", "Attack");
	t.append_child(*t.breadth_search("Ranged"), "", "Weapon 1");
	t.append_child(*t.breadth_search("Ranged"), "", "Weapon 2");
	t.append_child(*t.breadth_search("Ranged"), "", "Weapon 3");
	
	cout << t.breadth_search("Idle")->get_print() << endl;
	xml_parse_class x;
	x.parse_into_tree("/home/stoplime/ai_class/tree_project/test.xml");
	return 0;
}
