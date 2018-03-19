#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include <fstream>
#include <iterator>

#define NUMKEYS 99
#define FANOUT 100

typedef std::tuple<std::string, std::vector<std::string>> wordTup;

struct Node{
  wordTup words[NUMKEYS];
  // std::string words[NUMKEYS];
  // std::vector<std::string> docVec;
  Node* children[FANOUT];
  bool isLeaf;
  int currentNumKeys;
};

class BPlusTreeIndex {
  std::string indexFileName;
  Node* root;
  int pageSize;

  public:
    BPlusTreeIndex(std::string fileName, int paramPageSize): indexFileName(fileName), pageSize(paramPageSize){}

    Node* createTree();
    Node* createLeafNode();
    void insert(std::string word, std::vector<std::string> docs);
    void sortKeys(wordTup* wordDocTuples, int numKeys);
    wordTup split(Node* x, int i);
    void traverse(Node* p); 
};
