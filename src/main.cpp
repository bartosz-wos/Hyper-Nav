#include "SuffixAutomaton.h"

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <map>
#include <algorithm>
#include <set>

namespace fs = std::filesystem;

constexpr char SEPARATOR = '\1';

void print_help(const std::string& prog_name){
	std::cout << "Hyper-Nav - Blazing Fast File Search\n"
		  << "Usage: " << prog_name << " [OPTIONS] [QUERY]\n\n"
		  << "Options:\n"
		  << "   --help, -h          Display this help message and exit\n"
		  << "   --version, -v       Display version information and exit\n"
		  << "   -k NUM              Max edit distance for Fuzzy Search (default: 0)\n\n"
		  << "If QUERY is provided, the program performs a single search and exits.\n"
		  << "Otherwise, it enters interactive mode.\n";
}

int main(int argc, char* argv[]){
	std::string root_path = ".";

	int k_errors = 0;
	std::string single_query = "";
	bool interactive = true;

	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];

		if(arg == "--help" || arg == "-h"){
			print_help(argv[0]);
			return 0;
		}else if(arg == "--version" || arg == "-v"){
			std::cout << "Hyper-Nav v1.0.0\n";
			return 0;
		}else if(arg == "-k"){
			if(i + 1 < argc){
				i++;
				k_errors = std::stoi(argv[i]);
			}else{
				std::cerr << "Error: Flag -k requires a number!\n";
				return 1;
			}
		}else{
			single_query = arg;
			interactive = false;
		}
	}

	std::cout << "Indexing: " << root_path << " ..." << std::endl;
	if(k_errors > 0){
		std::cout << "Fuzzy Search active! Max edit distance: " << k_errors << std::endl;
	}

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

	std::vector<long long> left_sep(super_string.size());
	std::vector<long long> right_sep(super_string.size());
	
	left_sep[0] = -1;
	for(size_t i = 1; i < super_string.size(); i++){
		if(super_string[i] == SEPARATOR)
			left_sep[i] = i;
		else
			left_sep[i] = left_sep[i-1];
	}

	right_sep[(int)super_string.size()-1] = (int)super_string.size();
	for(int i = (int)super_string.size() - 2; i >= 0; i--){
		if(super_string[i] == SEPARATOR)
			right_sep[i] = i;
		else
			right_sep[i] = right_sep[i+1];
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

	std::cout << "Building Suffix Link Tree..." << std::endl;
	
	build_tree();

	std::cout << "__________________________" << std::endl;

	auto process_query = [&](const std::string& query){
		int cur = 0;
		bool found = true;

		for(char c : query){
			if(st[cur].next.find(c) == st[cur].next.end()){
				found = false;
				break;
			}
			cur = st[cur].next[c];
		}

		if(!found){
			std::cout << "   [X] Not found.\n";
		}else{
			std::vector<long long> all_positions;
			get_occurences(cur, all_positions);

			std::set<std::string> unique_files;

			for(const long long& pos : all_positions){
				long long start_pos = left_sep[pos] + 1;
				long long actual_end = right_sep[pos] - 1;

				std::string filename = super_string.substr(start_pos, actual_end - start_pos + 1);
				unique_files.insert(filename);
			}

		       	std::cout << "   [V] Found in: " << unique_files.size() << " files:" << std::endl;
			for(const std::string& file : unique_files)
				std::cout << "      - " << file << std::endl;
		}
	};

	if(!interactive){
		std::cout << "Searching for: '" << single_query << "'\n";
		process_query(single_query);
	}else{
		std::cout << "\n[READY] Type substring to search (or 'exit'):" << std::endl;
		std::string query;
		while(true){
			std::cout << "> ";
			if(!(std::cin >> query) || query == "exit")
				break;
			process_query(query);
		}
	}

	return 0;
}
