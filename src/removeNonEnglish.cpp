#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

/*
	Program to remove non_English docs from yelp dataset
*/
int main(int argc, char** argv)
{
	if(argc != 3){
		std::cerr<<"Usage: removeNonEnglish [text file] [output file]";
		std::exit(1);
	}

	fstream inputFile;
	ofstream outputFile;
	string input = argv[1];
	string output = argv[2];

	inputFile.open(input);
  	outputFile.open(output);

  	if(inputFile.is_open()){
	  	std::string line;
	    int breakflag = 0;
	  	//start reading file line by line
	  	while(getline(inputFile, line)){
	  		//get a vector of all the words in the line
	  		std::istringstream iss(line);
	  		std::vector<std::string> words((std::istream_iterator<std::string>(iss)),
	  										std::istream_iterator<std::string>());

	  		//current document is first entry in words
	  		std::string currentDoc = words[0];
	    
	  		//loop to get rid of docs that contain non-English chars
	  		for(size_t i = 1; i < words.size(); i++){
	  			std::string currentWord = words[i];
		        for(size_t j = 0; j < currentWord.length(); j++){
		          if(currentWord[j] < 33 || currentWord[j] > 126){
		            breakflag = 1;
		            break; //break out of letter for loop
		          }
	        	}
	        	if(breakflag == 1) break; //break out of words for loop
	  		}
	      	if(breakflag == 1) {
	        	breakflag = 0;
	        	continue; //continue to next line
	      	}
	      	else
	      		outputFile << line << endl;
	  	}
	}
  else
  	std::cerr<<"Unable to open file\n";
  inputFile.close();
  outputFile.close();
 }