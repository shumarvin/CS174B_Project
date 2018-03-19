#include "console.hpp"
#include <iostream>

void Console::showConsole(){
  while (true) {
    std::cout << "---|Please choose one of the following: " << std::endl;
    std::cout << "---|1. load [index_file]" << std::endl;
    std::cout << "---|2. merge [index_file]" << std::endl;
    std::cout << "---|3. quit" << std::endl;
    std::cout << "---|4. insert [document_name]" << std::endl;
    std::cout << "---|5. delete [document_name]" << std::endl;
    std::cout << "---|6. count [keyword]" << std::endl;
    std::cout << "---|7. search [keyword1, keyword2, ...]" << std::endl;
    std::cout << "---|8. printpath [keyword]" << std::endl;
    std::cout << "---|9. page [i]" << std::endl;
    std::cout << "---|10. range [keyword1, keyword2]" << std::endl;
    std::cout << std::endl << "Input: ";
    int input;
    std::cin >> input;
    std::cout << std::endl;

    switch(input) {
      case 1: loadIndex(); break;
      case 2: mergeIndex(); break;
      case 3: quit(); break;
      case 4: insertDoc(); break;
      case 5: deleteDoc(); break;
      case 6: countWord(); break;
      case 7: searchWords(); break;
      case 8: printpathWord(); break;
      case 9: displayPageContents(); break;
      case 10: rangeQuery(); break;
    }
  }
}

void Console::loadIndex(){
  std::cout << "---Loading Index" << std::endl;
  std::cout << std::endl << "Enter index file name: ";
  std::string indexFileName;
  std::cin >> indexFileName;
  std::cout << std::endl;
  bptIndex = new BPlusTreeIndex(indexFileName, pageSize);
  bptIndex->createTree();
  std::cout << "---Tree created" << std::endl; 
}

void Console::mergeIndex(){
  std::cout << "---in mergeIndex" << std::endl;
}

void Console::quit(){
  std::cout << "---Exiting Console" << std::endl;
  exit(0);
}

void Console::insertDoc(){
  std::cout << "---in insertDoc" << std::endl;
}

void Console::deleteDoc(){
  std::cout << "---in deleteDoc" << std::endl;
}

void Console::countWord(){
  std::cout << "---in countWord" << std::endl;
}

void Console::searchWords(){
  std::cout << "---in searchWords" << std::endl;
}

void Console::printpathWord(){
  std::cout << "---in printpathWord" << std::endl;
}

void Console::displayPageContents(){
  std::cout << "---in displayPageContents" << std::endl;
}

void Console::rangeQuery(){
  std::cout << "---in rangeQuery" << std::endl;
}
