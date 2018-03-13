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
	std::string spaces(pageSize, '0');
	outputFile << spaces;

	//write in current page
	outputFile.seekp(0);
	outputFile << currentPage<< " "<<std::endl;

	//set header and index offsets
	headerOffset = (int)outputFile.tellp()-1;

	outputFile.seekp(0, std::ios::end);
	bucketOffset = outputFile.tellp();
	std::cout << "bucketOffset at page creation: " << bucketOffset << std::endl;

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

		Header Entry is the word, and its offset into the page, separated by a comma.
	*/
	int headerEntrySize = word.length() + sizeof(',') + sizeof(pageSize) + sizeof(" ");
	// int nextBucketPagePointer = 10;
	std::string nextBucketPagePointer = "0000000";
	int bucketSize = word.length() + nextBucketPagePointer.length() + doc.length() + 2 * sizeof(' ') + sizeof('\n');
	std::cout << "bucketSize: " << bucketSize << ", for word: " << word << std::endl;

	std::tuple<int,int> currentPageOffsets = pageOffsets.at(currentPage-1);

	//if no room, make a new page
	if(bucketSize + headerEntrySize > std::get<1>(currentPageOffsets) - std::get<0>(currentPageOffsets) - 1){
		createNewPage();
		currentPageOffsets = pageOffsets.at(currentPage-1);
	}

	/*
		Write header entry
	*/
	int newBucketOffset = std::get<1>(currentPageOffsets) - bucketSize;

	outputFile.seekp(std::get<0>(currentPageOffsets), std::ios::beg);
	outputFile<< word<<","<<newBucketOffset<<" ";
	headerOffset = outputFile.tellp();


	/*
		Write bucket entry
	*/
	outputFile.seekp(std::get<1>(currentPageOffsets), std::ios::beg);
	outputFile.seekp(-bucketSize, std::ios::cur);
	bucketOffset = outputFile.tellp();
	std::cout << "bucketOffset: " << bucketOffset << ", for word: " << word << std::endl;
	outputFile << std::endl << word << " " << nextBucketPagePointer << " " << doc;

	//update pageOffsets vector
	pageOffsets[currentPage-1] = std::tuple<int,int>(headerOffset, bucketOffset);
}
/*
	Adds an existing word to the index
	pageNum (page number holding first bucket instance of word)
*/
void PageWriter::addExistingWord(std::string word, std::string doc, int pageNum){
	/*
		First, check if page has enough room.
	*/
	std::tuple<int,int> pageNumOffsets = pageOffsets.at(pageNum-1);
	int bucketEntrySize = doc.length() + sizeof(" ");

	//Checking first line of pageNum for offset within page
	std::ifstream inputFile(outputFileName);
	inputFile.seekg(pageNum * pageSize);
	std::string line;
	getline(inputFile, line);
	int locationOffsetPair = line.find(word) + word.length() + 1;
	std::string tempString = "";
	for (int i = locationOffsetPair; i < line.length(); i++) {
		char temp = line[i];
		if (temp == ' ') {
			break;
		} else {
			tempString += temp;
		}
	}

	//Going to word bucket using offset to get nextBucketPagePointer
	inputFile.seekg(stoi(tempString));
	getline(inputFile, line);
	int lbp = word.length() + 1;
	for (int i = lbp; i < line.length(); i++) {
		char temp = line[i];
		if (temp == ' ') {
			break;
		} else {
			tempString += temp;
		}
	}
	//If nextBucketPagePointer != 0, recursive call on next page
	if (stoi(tempString) != 0) {
		addExistingWord(word, doc, stoi(tempString));
	}
	//Check to see if there is room on currentpage


	// if (bucketEntrySize > std::get<1>(pageNumOffsets) - std::get<0>(pageNumOffsets) - 1) {
	// 	createNewPage();
	// }
}
