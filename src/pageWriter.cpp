#include "pageWriter.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>

/*
	Create new page with page number
*/
void PageWriter::createNewPage(){
	//update current page counter
	currentPage++;
	std::cout << "Creating new page: " << currentPage << std::endl; 

	//fill page with empty space so we can seek anywhere
	std::string spaces(pageSize, ' ');
	outputFile.seekp(pageSize * currentPage); 
	outputFile << spaces;

	//write in current page
	outputFile.seekp(pageSize * currentPage);
	outputFile << currentPage<< " "<<std::endl;

	//set header and index offsets
	headerOffset = ((int)outputFile.tellp()-1) % pageSize;

	outputFile.seekp(pageSize * (currentPage+1));
	bucketOffset = pageSize; 
	// std::cout << "bucketOffset at page creation: " << bucketOffset << std::endl;
	outputFile.flush(); 

	std::cout << "headerOffset = " << headerOffset << ", bucketOffset = " << bucketOffset << std::endl; 
	pageOffsets.push_back(std::tuple<int,int>(headerOffset, bucketOffset));
}
/*
	Adds a new word to the index
*/
int PageWriter::addNewWord(std::string word, std::string doc){
	/*
		First, check to see if the word's bucket and its header
		entry will fit into the current page.

		Bucket consists of the word, a pointer to its next bucket, and
		the list of docs, space-separated.

		Header Entry is the word, and its offset into the page, separated by a comma.
	*/
	std::tuple<int,int> currentPageOffsets = pageOffsets.at(currentPage);
	std::cout << "currentPageOffsets" << std::endl; 
	std::cout << std::get<1>(currentPageOffsets) << std::endl; 
	std::cout << std::get<0>(currentPageOffsets) << std::endl; 
	std::string nextBucketPagePointer = "0000000";
	int bucketSize = word.length() + nextBucketPagePointer.length() + doc.length() + 2 * sizeof(' ') + sizeof('\n');
	int newBucketOffset = std::get<1>(currentPageOffsets) - bucketSize;
	int headerEntrySize = word.length() + sizeof('\xFB') + std::to_string(newBucketOffset).length() + sizeof(' ');
	std::cout << "bucketSize: " << bucketSize << ", for word: " << word << std::endl;


	//if no room, make a new page
	if(bucketSize + headerEntrySize > std::get<1>(currentPageOffsets) - std::get<0>(currentPageOffsets) - 1){
		std::cout << "Creating New Page in addNewWord" << std::endl; 
		std::cout << bucketSize << std::endl; 
		std::cout << headerEntrySize << std::endl; 
		std::cout << std::get<1>(currentPageOffsets) << std::endl; 
		std::cout << std::get<0>(currentPageOffsets) << std::endl; 
		std::cout << "for current page: " << currentPage << std::endl; 
		std::cout << "Creating new page in addNewWord" << std::endl; 
		createNewPage();
		currentPageOffsets = pageOffsets.at(currentPage);
		newBucketOffset = std::get<1>(currentPageOffsets) - bucketSize;
	}

	/*
		Write header entry
	*/
	std::cout << "newBucketOffset after calc: " << newBucketOffset << std::endl;

	outputFile.seekp(currentPage * pageSize + std::get<0>(currentPageOffsets));
	outputFile<< word<<"\xFB"<<newBucketOffset<<" "<< std::endl;
	headerOffset = ((int)outputFile.tellp()-1) % pageSize;


	/*
		Write bucket entry
	*/
	outputFile.seekp(currentPage * pageSize + newBucketOffset);
	// outputFile.seekp(-bucketSize, std::ios::cur);
	// bucketOffset = outputFile.tellp();
	std::cout << "bucketOffset: " << newBucketOffset << ", for word: " << word << std::endl;
	outputFile << word << ' ' << nextBucketPagePointer << ' ' << doc << std::endl;
	outputFile.flush(); 

	//update pageOffsets vector
	pageOffsets[currentPage] = std::tuple<int,int>(headerOffset, newBucketOffset);
	return currentPage; 
}
/*
	Adds an existing word to the index
	pageNum (page number holding first bucket instance of word)
*/
void PageWriter::addExistingWord(std::string word, std::string doc, int pageNum){
	/*
		First, check if page has enough room.
	*/

	std::tuple<int,int> pageNumOffsets = pageOffsets.at(pageNum);
	int bucketEntrySize = doc.length() + sizeof(' ');

	//Checking first line of pageNum for offset within page
	std::ifstream inputFile(outputFileName);
	inputFile.seekg(pageNum * pageSize);
	std::string line;
	getline(inputFile, line);
	int locationOffsetPair = line.find(" " + word + '\xFB') + word.length() + 2;
	std::string offsetInPage = "";
	for (int i = locationOffsetPair; i < line.length(); i++) {
		char temp = line[i];
		if (temp == ' ') {
			break;
		} else {
			offsetInPage += temp;
		}
	}

	//Going to word bucket using offset to get nextBucketPagePointer
	inputFile.seekg(pageNum * pageSize + stoi(offsetInPage));
	std::string thisSucks;
	getline(inputFile, thisSucks);
	std::istringstream CBSiss(thisSucks);
	std::vector<std::string> currentBucketSplit((std::istream_iterator<std::string>(CBSiss)),
			std::istream_iterator<std::string>());
	for (size_t i = 2; i < currentBucketSplit.size(); i++) {
		std::cout << currentBucketSplit[i] << " "; 
		if (currentBucketSplit[i].compare(doc) == 0) {
			std::cout << word << " already exists for doc " << doc << std::endl;
			return;
		}
	}
	//If nextBucketPagePointer != 0, recursive call on next page
	if (stoi(currentBucketSplit[1]) != 0) {
		addExistingWord(word, doc, stoi(currentBucketSplit[1]));
		return; 
	}

	//Update header entries to account for new bucket bytes
	std::istringstream iss(line);
	std::vector<std::string> tempHeaderVec((std::istream_iterator<std::string>(iss)),
			std::istream_iterator<std::string>());
	std::cout << "Recalculating offsets because of word: " << word << std::endl; 
	for (size_t i = tempHeaderVec.size()-1; i >= 0; i--) {
		std::string headerTuple = tempHeaderVec[i];
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(tempHeaderVec[i]);
		while (std::getline(tokenStream, token, '\xFB')) {
			tokens.push_back(token);
		}
		tokens[1] = std::to_string(stoi(tokens[1])-bucketEntrySize);
		tempHeaderVec[i] = tokens[0] + '\xFB' + tokens[1];
		// Break if we have reached word,offset tuple of word
		//  so we don't update any of the tuples after that tuple
		if (tokens[0].compare(word) == 0) {
			break;
		}
	}
	line = "";
	for (size_t i = 0; i < tempHeaderVec.size(); i++) {
		line += tempHeaderVec[i] + " ";
	}
	// std::cout << line << std::endl; 

	// If bucketEntry doesn't fit on current page
	if (bucketEntrySize > std::get<1>(pageNumOffsets) - line.length() - 1) {
		int tempNum = addNewWord(word, doc);
		int numLeadingZeroes = 7-countDigits(tempNum);
		std::string newNextBucketPointer = std::string(numLeadingZeroes, '0') + std::to_string(tempNum);
		outputFile.seekp(pageNum * pageSize + stoi(offsetInPage) + word.length() + 1, std::ios::beg);
		outputFile << newNextBucketPointer;
		return;
	}

	// Go to bucketOffset and start moving lines
	// Create a vector that holds all of the buckets including bucket with word
	// All of these buckets will need to be copied to position bucketOffset - bucketEntrySize
	inputFile.seekg(pageSize * pageNum + std::get<1>(pageNumOffsets));
	std::string currentBucket;
	std::vector<std::string> bucketVec;
	while (getline(inputFile, currentBucket)) {
		bucketVec.push_back(currentBucket);
		// Check if bucket is for word, if so, break
		if (word.compare(currentBucket.substr(0, currentBucket.find(" "))) == 0) {
			bucketVec.back() = currentBucket + ' ' + doc;
			break;
		}
	}
	std::string newBucketBlock = "";
	for (size_t i = 0; i < bucketVec.size(); i++) {
		newBucketBlock += bucketVec[i] + '\n';
	}

	outputFile.seekp(pageSize * pageNum + std::get<1>(pageNumOffsets) - bucketEntrySize);
	outputFile << newBucketBlock;
	outputFile.seekp(pageSize * pageNum);
	outputFile << line;
	outputFile.flush(); 
	pageOffsets[pageNum] = std::tuple<int,int>(line.length(), std::get<1>(pageNumOffsets) - bucketEntrySize);

	checkWords(pageNum); 
}

int PageWriter::countDigits(int number){
  int digits = 0;
  if (number < 0) digits = 1;
  while (number) {
    number /= 10;
    digits++;
  }
  return digits;
}

void PageWriter::checkWords(int pageNum){
	std::ifstream inputFile(outputFileName);
	std::string line; 
	for (int j = 0; j < currentPage; j++){
		pageNum = j; 
		inputFile.seekg(pageNum * pageSize); 
		getline(inputFile, line); 
		std::istringstream iss(line);
		std::vector<std::string> tempHeaderVec((std::istream_iterator<std::string>(iss)),
				std::istream_iterator<std::string>());
		for (size_t i = 1; i < tempHeaderVec.size(); i++) {
			std::string headerTuple = tempHeaderVec[i];
			std::vector<std::string> tokens;
			std::string token;
			std::istringstream tokenStream(tempHeaderVec[i]);
			while (std::getline(tokenStream, token, '\xFB')) {
				tokens.push_back(token);
			}
			inputFile.seekg(pageNum * pageSize + stoi(tokens[1])); 
			std::string temp; 
			getline(inputFile, temp); 
			if(temp.find(tokens[0]) != 0){
				std::cout << "----Offset is corrupted: " << temp << " " <<tokens[0] << "," << tokens[1] << std::endl; 
				exit(0); 
			}
		}
	}
}

void PageWriter::printAllOffsets(){
	for(int i = 0; i < pageOffsets.size(); i++){
		std::cout << "headerOffsets for page " << i << " = " << std::get<0>(pageOffsets[i]) << std::endl; 
		std::cout << "bucketOffsets for page " << i << " = " << std::get<1>(pageOffsets[i]) << std::endl; 
	}
}