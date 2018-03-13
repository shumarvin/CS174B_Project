#include "pageWriter.hpp"
#include <iostream>
#include <string>

/*
	Create new page with page number
*/
void PageWriter::createNewPage(){
	//update current page counter
	currentPage++;

	//fill page with empty space so we can seek anywhere
	std::string spaces(pageSize, ' ');
	outputFile << spaces<<std::endl;

	//write in current page
	outputFile.seekp(0);
	outputFile << currentPage<< " "<<std::endl;

	//set header and index offsets
	headerOffset = outputFile.tellp();
	
	outputFile.seekp(0, std::ios::end);
	bucketOffset = outputFile.tellp();

	pageOffsets.push_back(std::tuple<int,int>(headerOffset, bucketOffset));
}
/*
	Adds a new word to the index
*/
void PageWriter::addNewWord(std::string word, std::string doc){
	/*
		First, check to see if the word's bucket and its header
		entry will fit into the current page.

		Bucket consists of the word, a pointer to its next bucket, and
		the list of docs, space-separated.

		Header Entry is the word, and its offset into the page, separated
		by a comma. The offset will have the max number of digits, so we don't
		have to alter it if it decreases. For example, if the pageSize is 8096 
		bytes, and the offset is 30, it will be represented as 0030
	*/
	int headerEntrySize = sizeof(word) + sizeof(',') + sizeof(pageSize) + sizeof(" ");
	int nextBucketPagePointer = 000000;
	int bucketSize = sizeof(word) + sizeof(nextBucketPagePointer) + sizeof(doc) + 2 * sizeof(' ');

	std::tuple<int,int> currentPageOffsets = pageOffsets.at(currentPage-1);

	//if no room, make a new page
	if(bucketSize + headerEntrySize > std::get<1>(currentPageOffsets) - std::get<0>(currentPageOffsets)){
		createNewPage();
		currentPageOffsets = pageOffsets.at(currentPage-1);
	}
	
	/*
		Write header entry
	*/
	int newBucketOffset = std::get<1>(currentPageOffsets) - bucketSize;
	outputFile.seekp(headerOffset-1, std::ios::beg);
	int numZeroes = countDigits(newBucketOffset);
	outputFile<< word<<","<<'0'*(countDigits(pageSize) - numZeroes) << newBucketOffset<<" ";
	headerOffset = outputFile.tellp();


	/*
		Write bucket entry
	*/
	outputFile.seekp(newBucketOffset,std::ios::end);
	outputFile << word << " " << nextBucketPagePointer << " " << doc<<; 
	bucketOffset = outputFile.tellp();

	//update pageOffsets vector
	pageOffsets.at(currentPage-1) = std::tuple<int,int>(headerOffset, bucketOffset);
}
//count number of digits in num
int PageWriter::countDigits(int num){
	int digits = 0;
	while (num) {
        num /= 10;
        digits++;
    }
    return digits;
}