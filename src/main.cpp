#include "SearchEngine.h"

#include <iostream>
#include <string>

void print_help(const std::string& prog_name){
	std::cout << "Hyper-Nav - Blazing Fast File Search\n"
		  << "Usage: " << prog_name << " [OPTIONS] [QUERY]\n\n"
		  << "Options:\n"
		  << "   --help, -h          Display this help message and exit\n"
		  << "   --version, -v       Display version information and exit\n"
		  << "   -k NUM              Max edit distance for Fuzzy Search (default: 0)\n"
		  << "   -d, --dir PATH      Target directory to index (default: current directory '.')\n"
		  << "   -a, --all           Do not ignore hidden files and directories (starting with '.')\n"
		  << "   -t, --time          Display execution time for indexing and searching\n\n"
		  << "If QUERY is provided, the program performs a single search and exits.\n"
		  << "Otherwise, it enters interactive mode.\n";
}

int main(int argc, char* argv[]){
	std::string root_path = ".";

	long long k_errors = 0;
	std::string single_query = "";
	bool interactive = true;
	bool ignore_hidden = true;
	bool measure_time = false;

	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];

		if(arg == "--help" || arg == "-h"){
			print_help(argv[0]);
			return 0;
		}else if(arg == "--version" || arg == "-v"){
			std::cout << "Hyper-Nav v1.0.0\n";
			return 0;
		}else if(arg == "-a" || arg == "--all"){
			ignore_hidden = false;
		}else if(arg == "-t" || arg == "--time"){
			measure_time = true;
		}else if(arg == "-k"){
			if(i + 1 < argc){
				i++;
				k_errors = std::stoll(argv[i]);
			}else{
				std::cerr << "Error: Flag -k requires a number!\n";
				return 1;
			}
		}else if(arg == "-d" || arg == "--dir"){
			if(i + 1 < argc){
				i++;
				root_path = argv[i];
			}else{
				std::cerr << "Error: Flag -d requires a path!\n";
				return 1;
			}
		}else{
			single_query = arg;
			interactive = false;
		}
	}

	SearchEngine engine;
	engine.build_index(root_path, ignore_hidden, measure_time);

	if(k_errors > 0){
		std::cout << "Fuzzy Search active! Max edit distance: " << k_errors << std::endl;
	}
	
	if(!ignore_hidden){
		std::cout << "Hidden files inclusion: ACTIVE\n";
	}

	if(!interactive){
		std::cout << "Searching for: '" << single_query << "'\n";
		engine.search(single_query, k_errors);
	}else{
		std::cout << "\n[READY] Type substring to search (or 'exit'):" << std::endl;
		std::string query;
		while(true){
			std::cout << "> ";
			if(!(std::cin >> query) || query == "exit")
				break;
			engine.search(query, k_errors, measure_time);
		}
	}

	return 0;
}
