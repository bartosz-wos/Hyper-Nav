#include "SearchEngine.h"
#include "SuffixAutomaton.h"

#include <iostream>
#include <filesystem>
#include <algorithm>
#include <chrono>

namespace fs = std::filesystem;
constexpr char SEPARATOR = '\1';

SearchEngine::SearchEngine() : super_string(""), file_count(0){}

void SearchEngine::build_index(const std::string& root_path, bool ignore_hidden, bool measure_time){
	auto start_time = std::chrono::high_resolution_clock::now();

	std::cout << "Indexing: " << root_path << " ..." << std::endl;

	try{
		fs::recursive_directory_iterator it(root_path, fs::directory_options::skip_permission_denied);
		fs::recursive_directory_iterator end;

		while(it != end){
			std::string current_filename = it->path().filename().string();

			if(ignore_hidden && !current_filename.empty() && current_filename[0] == '.'){
				if(current_filename != "." && current_filename != ".."){
					if(it->is_directory()){
						it.disable_recursion_pending();
					}
					++it;
					continue;
				}
			}

			if(it->is_regular_file()){
				std::string path = it->path().string();
				super_string += path;
				super_string += SEPARATOR;
				file_count++;
			}
			++it;
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
	for(long long i = 1; i < (long long)super_string.size(); i++){
                if(super_string[i] == SEPARATOR)
                        left_sep[i] = i;
                else
                        left_sep[i] = left_sep[i-1];
        }

	right_sep[(long long)super_string.size()-1] = (long long)super_string.size();
        for(long long i = (long long)super_string.size() - 2; i >= 0; i--){
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

        for(long long i = 0; i < (long long)super_string.size(); i++){
                sa_extend(super_string[i], i);
        }

        std::cout << "Done. Automaton Nodes: " << sz << std::endl;
        std::cout << "__________________________" << std::endl;

        std::cout << "Building Suffix Link Tree..." << std::endl;

        build_tree();

        std::cout << "__________________________" << std::endl;

	auto end_time = std::chrono::high_resolution_clock::now();

	if(measure_time){
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
		std::cout << "Indexing & Tree Building Time: " << duration << " ms" << std::endl;
		std::cout << "__________________________" << std::endl;
	}
}

void SearchEngine::dfs_levenshtein(long long u, long long q_idx, long long errors, const std::string& query, long long max_errors,
		                   std::set<long long>& valid_states, std::set<std::tuple<long long, long long, long long>>& visited){
	if(errors > max_errors)
		return;

	auto state = std::make_tuple(u, q_idx, errors);
	if(visited.count(state))
		return;
	visited.insert(state);

	if(q_idx == (long long)query.size()){
		valid_states.insert(u);
	}

	if(q_idx < (long long)query.size()){
		dfs_levenshtein(u, q_idx + 1, errors + 1, query, max_errors, valid_states, visited);
	}

	for(const auto& [edge_letter, v] : st[u].next){
		if(edge_letter == SEPARATOR)
			continue;
		if(q_idx < (long long)query.size()){
			if(query[q_idx] == edge_letter){
				dfs_levenshtein(v, q_idx + 1, errors, query, max_errors, valid_states, visited);
			}else{
				dfs_levenshtein(v, q_idx + 1, errors + 1, query, max_errors, valid_states, visited);
			}
		}
		dfs_levenshtein(v, q_idx, errors + 1, query, max_errors, valid_states, visited);
	}
}

void SearchEngine::search(const std::string& query, long long k_errors, bool measure_time){
	auto start_time = std::chrono::high_resolution_clock::now();

	std::set<long long> valid_states;
	std::set<std::tuple<long long, long long, long long>> visited;

	dfs_levenshtein(0, 0, 0, query, k_errors, valid_states, visited);

	if(valid_states.empty()){
		std::cout << "   [X] Not found.\n";
	}else{
		std::vector<long long> all_positions;
		for(long long state : valid_states)
			get_occurences(state, all_positions);

		std::set<std::string> unique_files;

		for(const long long& pos : all_positions){
			long long start_pos = left_sep[pos] + 1;
			long long actual_end = right_sep[pos] - 1;

			if(start_pos <= actual_end){
				std::string filename = super_string.substr(start_pos, actual_end - start_pos + 1);
				unique_files.insert(filename);
			}
		}

		std::cout << "   [V] Found in: " << unique_files.size() << " files:" << std::endl;
		for(const std::string& file : unique_files)
			std::cout << "      - " << file << std::endl;
	}

	auto end_time = std::chrono::high_resolution_clock::now();

	if(measure_time){
		auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
		double duration_ms = duration_us / 1000.0;
		std::cout << "   [TIME] " << duration_us << " us (" << duration_ms << " ms)\n";
	}
}
