#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <string>
#include <vector>
#include <set>

class SearchEngine{
private:
	std::string super_string;
	std::vector<long long> left_sep;
	std::vector<long long> right_sep;
	long long file_count;

	void dfs_levenshtein(long long u, long long q_idx, long long errors, const std::string& query, long long max_errors, 
		             std::set<long long>& valid_states, std::set<std::tuple<long long, long long, long long>>& visited);

public:
	SearchEngine();

	void build_index(const std::string& root_path);
	void search(const std::string& query, long long k_errors);
};

#endif
