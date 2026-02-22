#include "SearchEngine.h"
#include "SuffixAutomaton.h"

#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;
constexpr char SEPARATOR = '\1';

SearchEngine::SearchEngine() : super_string(""), file_count(0){}

void SearchEngine::build_index(const std::string& root_path){
	std::cout << "Indexing: " << root_path << " ..." << std::endl;

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

	if(super_string.empty()){
		std::cout << "No files found in " << root_path << std::endl;
		return;
	}

	left_sep.resize(super_string.size());
	right_sep.resize(super_string.size());

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
}

void SearchEngine::search(const std::string& query, int k_errors){
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
}
