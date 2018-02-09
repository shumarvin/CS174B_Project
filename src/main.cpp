#include <iostream>

int main(int argc, char**argv)
{
	if(argc < 5)
	{
		std::cerr<<"Usage: build index [text file] [index file] -p [page size]\n";
		return 1;
	}

	for(int i = 0; i < argc; i++)
	{
		std::cout<<argv[i]<<std::endl;
	}
	return 0;
}