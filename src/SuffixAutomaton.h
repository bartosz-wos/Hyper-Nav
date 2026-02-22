#ifndef SUFFIX_AUTOMATON_H
#define SUFFIX_AUTOMATON_H

#include <vector>
#include <map>
#include <string>

struct State{
	long long len, link;
	long long first_pos = -1;
	std::map<char, long long> next;
};

extern std::vector<State> st;
extern long long sz;
extern long long last;
extern std::vector<std::vector<long long>> inv_link;

void sa_init();
void sa_extend(char c, long long current_pos);
void build_tree();
void get_occurences(long long v, std::vector<long long>& positions);

#endif
