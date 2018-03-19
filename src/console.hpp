#ifndef CONSOLE_HPP
#define CONSOLE_HPP
#include "bPlusTreeIndex.hpp"

class Console {
  int numBufferPages;
  int pageSize;
  BPlusTreeIndex* bptIndex;

  public:
    Console(int numPages, int paramPageSize): numBufferPages(numPages), pageSize(paramPageSize){}

    void showConsole();
    void loadIndex();
    void mergeIndex();
    void quit();
    void insertDoc();
    void deleteDoc();
    void countWord();
    void searchWords();
    void printpathWord();
    void displayPageContents();
    void rangeQuery();
};

#endif
