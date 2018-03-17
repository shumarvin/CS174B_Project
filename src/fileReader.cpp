#include "fileReader.hpp"
#include "pageWriter.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <map>

void FileReader::parseFile(){
  std::fstream input;
  std::ofstream output;
  std::map<std::string, int> wordToPageMap;
  std::map<std::string, int>::iterator it;

  //open files
  input.open(inputFile);
  output.open(outputFile);

  //instantiate PageWrite object to write to index file
  PageWriter pageWriter(output, pageSize, outputFile);
  pageWriter.createNewPage();

  if(input.is_open()){
  	std::string line;

  	//start reading file line by line
  	while(std::getline(input, line)){
  		//get a vector of all the words in the line
  		std::istringstream iss(line);
  		std::vector<std::string> words((std::istream_iterator<std::string>(iss)),
  										std::istream_iterator<std::string>());

  		//current document is first entry in words
  		std::string currentDoc = words[0].substr(3);
      std::cout << "Processing document: " << currentDoc << std::endl; 
    	//loop to add words to index
    	for(size_t i = 1; i < words.size(); i++){
        std::string currentWord = words[i];
        if (currentWord.length() > 256) {
          continue;
        }
        // std::cout << "--- " << currentWord << " in fileReader.cpp ---" << std::endl;
        pageWriter.printAllOffsets();
        it = wordToPageMap.find(currentWord);
        if(it == wordToPageMap.end()){
          pageWriter.addNewWord(currentWord, currentDoc);
        	wordToPageMap.insert(std::pair<std::string, int>(currentWord, pageWriter.getCurrentPage()));
        }
        else{
          pageWriter.addExistingWord(currentWord, currentDoc, it->second);
        }
        // std::cout << "--- End of adding word: " << currentWord << " ---\n" <<std::endl;
    	}
  	}
  }

  else
  	std::cerr<<"Unable to open file\n";

  input.close();
  output.close();

}
