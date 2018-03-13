#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv){
	
	ifstream inputFile(argv[1]);
	string line;
	getline(inputFile, line);
	cout<<line<<endl;
}