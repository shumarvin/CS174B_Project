#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char** argv){

	ofstream testStream(argv[1]);
	testStream << "super" << " " << 0 << " " << "0" << std::endl;

	ifstream inputFile(argv[1]);
	string line;
	getline(inputFile, line);
	cout<<line<<endl; 
	cout<<line.length()<<endl;
}
