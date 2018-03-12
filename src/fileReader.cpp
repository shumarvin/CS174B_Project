#include "fileReader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <map>



int testhash(const std::string &s)
{
  int seed = 131; 
  int hash = 13;
  int length = s.length();
  for(int i = 0; i < length; i++)
  {
    hash = (hash * seed) + s[i];
  }
  return hash;
}

void genPageHeader(int pageNum, char* memBlock){
	//memBlock = new char[]
}
void FileReader::parseFile(){
  std::fstream input; 
  std::ofstream output;
  //std::ofstream mapOutput;
  int currentPage = 1;

  input.open(inputFile);
  output.open(outputFile, std::ios::binary);
  //mapOutput.open("wordToPageMap");

  //write header for first page
  output<<currentPage<<" ";
  pageSize -= sizeof(currentPage);
  if(input.is_open()){
  	std::string line;
  	std::hash<std::string> h;
  	std::map<std::string, int> wordToPageMap; 
  	std::map<std::string, int>::iterator it;
    int collisionCounter = 0;
  	std::string max;
    int breakflag = 0;
  	//start reading file line by line
  	while(std::getline(input, line)){
  		//get a vector of all the words in the line
  		std::istringstream iss(line);
  		std::vector<std::string> words((std::istream_iterator<std::string>(iss)),
  										std::istream_iterator<std::string>());

  		//current document is first entry in words
  		std::string currentDoc = words[0];
    
      
      	//loop to add words to index
      	for(size_t i = 1; i < words.size(); i++){
	        std::string currentWord = words[i];
	        // if(currentWord.length() > max.length()){
	        //   max = currentWord;
	        //   std::cout<<currentDoc<<" "<<currentWord<<std::endl;
	        // }
	        it = wordToPageMap.find(currentWord);
	        if(it == wordToPageMap.end()){
	        	wordToPageMap.insert(std::pair<std::string, int>(currentWord, 1));
	        }
	        else{
	        	
	        }
	    //     int test = h(currentWord);
       
  			// ret = uniqueWords.insert(std::pair<int,std::string>(test, currentWord));
  			// if(ret.second == false){
  			// 	if(uniqueWords.at(test).compare(currentWord) == 0)
  			// 		continue;
     //      		collisionCounter++;
  			// 	std::cout<<"Found collision.\n";
  			// 	std::cout<<currentWord<<" "<<test<<std::endl;
  			// 	std::cout<<uniqueWords.at(test)<<std::endl;
		   //      std::cout<<"Collisions found: "<<collisionCounter<<std::endl;
		   //      std::cout<<std::endl;
  			// }
      	}	
  	}
  	// std::cout<<max<<std::endl;
  	// std::cout<<"max length: "<<max.length()<<std::endl;
  	// std::cout<<"max size: "<<max.size()<<std::endl;

  	//for(auto& kv: wordToPageMap) mapOutput << kv.second;
  }
  
  else
  	std::cerr<<"Unable to open file\n";

  input.close();
  output.close();
 //mapOutput.close();
}


