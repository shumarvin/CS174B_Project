#ifndef PAGE_WRITER_HPP
#define PAGE_WRITER_HPP
#include <iostream>
#include <fstream>

class PageWriter{
	std::ofstream& outputFile;
	int pageSize, currentPage, headerOffset, indexOffset;

public:
	PageWriter(std::ofstream &output, int page): outputFile(output), pageSize(page), currentPage(0), headerOffset(0), indexOffset(0){}

	void createNewPage();

};


#endif