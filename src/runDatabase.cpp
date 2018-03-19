#include "console.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char const *argv[]) {

  //check for invalid usage
	if(argc < 2 || argc > 4 || argc == 3) {
		std::cerr<<"Usage: runDatabase [page size(in bytes)] -b [number of buffer pages]\n";
		std::exit(1);
	}

  if (argc == 2) {
    Console console(8096, atoi(argv[1]));
    console.showConsole();
  } else {
    Console console(atoi(argv[3]), atoi(argv[1]));
    console.showConsole();
  }

  return 0;
}
