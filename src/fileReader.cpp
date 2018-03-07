#include "fileReader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

void FileReader::parseFile(){
  std::fstream input; 
  std::ofstream output;

  input.open(inputFile);
  output.open(outputFile);

  if(input.is_open()){
  	std::string line;
  	//start reading file line by line
  	while(std::getline(input, line)){
  		//get a vector of all the words in the line
  		std::istringstream iss(line);
  		std::vector<std::string> words((std::istream_iterator<std::string>(iss)),
  										std::istream_iterator<std::string>());

  		//current document is first entry in words
  		std::string currentDoc = words[0];

  		//process each word in the line
  		for(size_t i = 1; i < words.size(); i++){
  			std::string currentWord = words[i];
  		}
  	}
  }
  else
  	std::cerr<<"Unable to open file\n";
  input.close();
  output.close();
  
}


