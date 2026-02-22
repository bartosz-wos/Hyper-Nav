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

public:
	SearchEngine();

	void build_index(const std::string& root_path);
	void search(const std::string& query, int k_errors);
};

#endif
