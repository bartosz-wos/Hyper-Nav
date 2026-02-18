#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include <algorithm>

namespace fs = std::filesystem;

constexpr char SEPARATOR = '\1';

struct State{
	int len, link;
	std::map<char, int> next;
	long long first_pos = -1;
};

std::vector<State> st;
int sz, last;

void sa_init(){
	st.clear();
	st.push_back({0, -1, {}, -1});
	sz = 1;
	last = 0;
}

void sa_extend(char c, long long current_pos){
	int cur = sz++;
	st.emplace_back();

	st[cur].len = st[last].len + 1;
	st[cur].first_pos = current_pos;
	st[cur].next.clear();
	st[cur].link = -1;

	int p = last;
	while(p != -1 && st[p].next.find(c) == st[p].next.end()){
		st[p].next[c] = cur;
		p = st[p].link;
	}
	if(p == -1){
		st[cur].link = 0;
	}else{
		int q = st[p].next[c];
		if(st[p].len + 1 == st[q].len){
			st[cur].link = q;
		}else{
			int clone = sz++;
			st.emplace_back();
			st[clone].len = st[p].len + 1;
			st[clone].next = st[q].next;
			st[clone].link = st[q].link;
			st[clone].first_pos = st[q].first_pos;
			while(p != -1 && st[p].next[c] == q){
				st[p].next[c] = clone;
				p = st[p].link;
			}
			st[q].link = st[cur].link = clone;
		}
	}
	last = cur;
}

int main(){
	std::string root_path = ".";

	std::cout << "Indexing: " << root_path << " ..." << std::endl;

	std::string super_string = "";
	long long file_count = 0;

	try{
		for(const auto& entry : fs::recursive_directory_iterator(root_path)){
			if(entry.is_regular_file()){
				std::string path = entry.path().string();
				super_string += path;
				super_string += SEPARATOR;
				file_count ++;
			}
		}
	} catch (const fs::filesystem_error& e){
		std::cerr << "Error: " << e.what() << std::endl;
	}

	std::cout << "__________________________" << std::endl;
	std::cout << "Indexing complete." << std::endl;
	std::cout << "Files found: " << file_count << std::endl;
	std::cout << "Total Index Size: " << super_string.size() << "bytes" << std::endl;
	std::cout << "__________________________" << std::endl;

	std::cout << "--- DATA PREVIEW (First 200 chars) ---" << std::endl;
	for(size_t i = 0; i < std::min<size_t>(200, super_string.size()); i++){
		if(super_string[i] == SEPARATOR){
			std::cout << "[SEP]";
		}else{
			std::cout << super_string[i];
		}
	}
	std::cout << "\n__________________________" << std::endl;

	std::cout << "Building Suffix Automaton..." << std::endl;
	
	st.reserve(super_string.size() * 2);
	sa_init();
	
	for(size_t i = 0; i < super_string.size(); i++){
		sa_extend(super_string[i], i);
	}

	std::cout << "Done. Automaton Nodes: " << sz << std::endl;
	std::cout << "__________________________" << std::endl;

	std::cout << "\n[READY] Type substring to search (or 'exit'):" << std::endl;

	std::string query;
	while(true){
		std::cout << "> ";
		if(!(std::cin >> query) || query == "exit")
			break;

		int cur = 0;
		bool found = true;

		for(char c : query){
			if(st[cur].next.find(c) == st[cur].next.end()){
				found = 0;
				break;
			}
			cur = st[cur].next[c];
		}

		if(!found){
			std::cout << "   [X] Not found." << std::endl;
		}else{
			long long end_pos = st[cur].first_pos;
			long long start_pos = end_pos;
			
			while(start_pos >= 0 && super_string[start_pos] != SEPARATOR){
				start_pos--;
			}
			start_pos++;

			long long actual_end = end_pos;
			while(actual_end < (long long)super_string.size() && super_string[actual_end] != SEPARATOR){
				actual_end++;
			}

			std::string filename = super_string.substr(start_pos, actual_end - start_pos);
			std::cout << "   [V] Found in: " << filename << std::endl;
		}
	}

	return 0;
}
