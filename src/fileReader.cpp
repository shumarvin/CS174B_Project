#include "fileReader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <map>
void FileReader::parseFile(){
  std::fstream input; 
  std::ofstream output;

  input.open(inputFile);
  output.open(outputFile);

  //write header for first page

  if(input.is_open()){
  	std::string line;
  	std::hash<std::string> h;
  	std::map<int,std::string> uniqueWords; 
  	std::pair<std::map<int,std::string>::iterator,bool> ret;
  	std::string max;
  	std::cout<<sizeof(std::string[63])<<std::endl;
  	//start reading file line by line
  	/*
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
  			if(currentWord.length() > max.length()) {
  				max = currentWord;
  				//std::cout<<max<<std::endl;
  			}
  			
  			ret = uniqueWords.insert(std::pair<int,std::string>(hash(currentWord), currentWord));
  			if(ret.second == false){
  				if(uniqueWords.at(hash(currentWord)).compare(currentWord) == 0)
  					continue;
  				std::cout<<"Found collision.\n";
  				std::cout<<currentWord<<" "<<hash(currentWord)<<std::endl;
  				std::cout<<uniqueWords.at(hash(currentWord))<<std::endl;
  				std::exit(0);
  			}
  			
  		}
  	}
  	std::cout<<max<<std::endl;
  	std::cout<<"max length: "<<max.length()<<std::endl;
  	std::cout<<"max size: "<<max.size()<<std::endl;
  	*/
  }
  
  else
  	std::cerr<<"Unable to open file\n";
  input.close();
  output.close();
  
}


