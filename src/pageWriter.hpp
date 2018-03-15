#ifndef PAGE_WRITER_HPP
#define PAGE_WRITER_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

class PageWriter{
private:
	std::ofstream& outputFile;
	std::vector<std::tuple<int,int>> pageOffsets;
	int pageSize, currentPage, headerOffset, bucketOffset;
	std::string outputFileName;

	int countDigits(int number);

public:
	PageWriter(std::ofstream &output, int page, std::string fileName):
		outputFile(output), pageSize(page), outputFileName(fileName), currentPage(-1), headerOffset(0), bucketOffset(0){}

	void createNewPage();
	void addNewWord(std::string word, std::string doc);
	void addExistingWord(std::string word, std::string doc, int pageNum);
	int getCurrentPage(){return currentPage;}
};


#endif
