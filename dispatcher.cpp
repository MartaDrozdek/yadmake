#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <vector>
#include <map>
#include "dbparser.hpp"
#include <boost/tokenizer.hpp>
#include <string>
#include <boost/foreach.hpp>
#include <string>
#include "commands.h"

using namespace std;

class Computer{};

void print(const char s[]){
   printf("%s\n",s);
}

void error(){
   print("error");
}

void print_main_leaf(DependencyGraph * graph){
   print("main:");
	BOOST_FOREACH(Target * t, graph->main_targets)
      print(t->name.c_str());

   print("leaf:");
	BOOST_FOREACH(Target * t, graph->leaf_targets)
      print(t->name.c_str());
   print("");
}

/* bierze target, wykonuje wszystkie komendy w command */
int realize(Target * t, Computer * c){
   
   print("realize:");
   print(t->name.c_str());
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	
	boost::char_separator<char> sep("\n");
   tokenizer tok(t->get_command(), sep);

	BOOST_FOREACH(string s, tok)
		system(s.c_str());
	
	return 0;
}

/* mark target as realized, 
 * check whether dependent targets are ready to realize,
 * add them to targets */
void mark_realized(Target * t, vector<Target*> & targets){

	t->realized = true;

   BOOST_FOREACH(Target * i, t->get_dependent_targets()){
      --(i->topo_ord);
      if (i->topo_ord == 0)
         targets.push_back(i);
   }
}

void init_free_comp(vector<Computer *> & free_comp){
   free_comp.push_back(NULL);
}

void dispatcher(){

	vector<Target *> targets;	// only these ready to make
	map<pid_t, Target *> targ;
	vector<Computer *> free_comp;
	map<pid_t, Computer *> comp;
	int child_count;
   vector<string> basics;
	// read data about available computers

	// connect with them (check connection or sth)

	// get graph
	DependencyGraph dependency_graph(0);
   print("dep graph");
   print_main_leaf(&dependency_graph);
   // get commands      TODO

/*   
   basics.push_back("make");
   count_commands(&dependency_graph, basics, "blah");
   print("count commands");
  */ 

   init_free_comp(free_comp);

	// init targets
	targets = dependency_graph.leaf_targets;

	// (proces dla każdego targetu)

	child_count = 0;
   
	while(!targets.empty() || child_count > 0){
		while (!targets.empty() && !free_comp.empty()){
			Target *t = targets.back();
			targets.pop_back();
			Computer *c = free_comp.back();
			free_comp.pop_back();

			pid_t who;

			switch (who = fork()){
				case	-1:
					error();
					break;
				case 	 0: 
					if (realize(t, c) != 0){
						error();
					}
               exit(0);
					break;
				default:	
					++child_count;
					comp[who] = c;
					targ[who] = t;
					break;
			}
		}

		int status;
		pid_t who;

		who = wait(&status);
		if (status != 0){
			error();
		}
		--child_count;

		free_comp.push_back(comp[who]);
		comp.erase(who);

		Target *t = targ[who];
		targ.erase(who);
		
		mark_realized(t, targets);
	}
}


/* tests */

/* realize */

void realize_test(){
	Target t("t1");
	Computer c;
	t.set_command("echo ala\necho alala\necho ololo");
	realize(&t, &c);
}

void jakis_test(){
   
}

