#ifndef SUFFIX_AUTOMATON_H
#define SUFFIX_AUTOMATON_H

#include <vector>
#include <map>
#include <string>

struct State{
	int len, link;
	long long first_pos = -1;
	std::map<char, int> next;
};

extern std::vector<State> st;
extern int sz;
extern int last;
extern std::vector<std::vector<int>> inv_link;

void sa_init();
void sa_extend(char c, long long current_position);
void build_tree();
void get_occurences(int v, std::vector<long long>& positions);

#endif
