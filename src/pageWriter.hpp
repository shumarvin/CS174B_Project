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

	int countDigits(int num);

public:
	PageWriter(std::ofstream &output, int page): outputFile(output), pageSize(page), currentPage(0), headerOffset(0), bucketOffset(0){}

	void createNewPage();
	void addNewWord(std::string word, std::string doc);
	int getCurrentPage(){return currentPage;}
};


#endif