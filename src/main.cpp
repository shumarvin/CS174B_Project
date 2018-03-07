#include "fileReader.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char**argv)
{
	//check for invalid usage
	if(argc < 3 || argc > 5 || argc == 4)
	{
		std::cerr<<"Usage: build_index [text file] [index file] -p [page size]\n";
		std::exit(1);
	}
	
	std::string input(argv[1]);
	std::string output(argv[2]);
	if(argc == 5)
	{
		int page_size = strtol(argv[4], NULL, 10);
		FileReader reader(input,output,page_size);
		reader.parseFile();
	}
	else
	{
		FileReader reader(input,output,8096);
		reader.parseFile();
	}
	return 0;
}