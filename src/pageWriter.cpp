#include "pageWriter.hpp"

void PageWriter::createNewPage(){
	currentPage++;

	char* currentPageBlock = new char[1];
	currentPageBlock[0] = currentPage;
	outputFile.write(currentPageBlock, sizeof(currentPageBlock));
	headerOffset = outputFile.tellp();
	pageSize -= sizeof(currentPageBlock);
}