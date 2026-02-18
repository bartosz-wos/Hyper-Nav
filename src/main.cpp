#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

constexpr char SEPARATOR = '\1';

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

	return 0;
}
