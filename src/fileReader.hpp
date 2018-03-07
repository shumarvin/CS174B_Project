#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <string>

class FileReader {
	std::string inputFile, outputFile;
	int pageSize;
	
	public:
		//constructor
		FileReader(std::string input, std::string output, int p): inputFile(input), outputFile(output), pageSize(p){}

		void parseFile();

  
};


#endif
