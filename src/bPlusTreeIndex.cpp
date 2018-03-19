#include "bPlusTreeIndex.hpp"
#include <iostream>

Node* BPlusTreeIndex::createTree(){
  std::ifstream input;
  input.open(indexFileName);
  if (input.is_open()) {
    std::cout << "File opened" << std::endl;
    int fileSize = input.tellg();
    input.seekg(0, std::ios::end);
    fileSize = (int)input.tellg() - fileSize;
    int numPages = fileSize/pageSize;
    std::cout << "number of pages in file: " << numPages << std::endl;
    input.seekg(0);
    std::string tempLine;
    for (int i = 0; i < numPages; i++) {
      input.seekg(pageSize*i);
      getline(input, tempLine);
      // std::cout << tempLine << std::endl;
      std::istringstream streamHeader(tempLine);
  	  std::vector<std::string> headerEntries((std::istream_iterator<std::string>(streamHeader)),
  			std::istream_iterator<std::string>());
      for (int j = 1; j < headerEntries.size(); j++) {
        std::vector<std::string> tokens;
    		std::string token;
    		std::istringstream tokenStream(headerEntries[j]);
    		while (std::getline(tokenStream, token, '\xFB')) {
          std::cout << token << std::endl;
    			tokens.push_back(token);
    		}
        input.seekg(pageSize*i + stoi(tokens[1]));
        std::string bucketLine;
        getline(input, bucketLine);
        std::cout << "bucketLine: " << bucketLine << std::endl; 
        std::istringstream bucketStream(bucketLine);
        std::vector<std::string> bucketEntryVec((std::istream_iterator<std::string>(bucketStream)),
  			   std::istream_iterator<std::string>());
        wordTup tempTuple(bucketEntryVec[0], std::vector<std::string>());
        for (int k = 2; k < bucketEntryVec.size(); k++) {
          std::get<1>(tempTuple).push_back(bucketEntryVec[k]);
        }
        insert(std::get<0>(tempTuple), std::get<1>(tempTuple));
      }
    }
    // Print the tree out after creation
    traverse(root);
    return root;
  } else {
    std::cerr << "Unable to open file" << std::endl;
    exit(0);
  }
}

Node* BPlusTreeIndex::createLeafNode(){
  Node* tempNode = new Node;
  tempNode->isLeaf = true;
  tempNode->currentNumKeys = 0;
  return tempNode;
}

void BPlusTreeIndex::insert(std::string word, std::vector<std::string> docs){
  Node* tempNode = root;
  if (tempNode == NULL) {
    root = createLeafNode();
    tempNode = root;
  } else {
    int i;
    // If node is a leaf node and it's keys are full, split that node
    if ((tempNode->isLeaf) && tempNode->currentNumKeys == NUMKEYS) {
      split(tempNode, -1);
      tempNode = root;
      for (i = 0; i < (tempNode->currentNumKeys); i++) {
        if ((word > std::get<0>(tempNode->words[i])) && (word < std::get<0>(tempNode->words[i+1]))) {
          i++;
          break;
        }
        else if (word < std::get<0>(tempNode->words[0])) break;
        else continue;
      }
      tempNode = tempNode->children[i];
    } else {
      while(tempNode->isLeaf == false){
        for (i = 0; i < tempNode->currentNumKeys; i++) {
          if ((word > std::get<0>(tempNode->words[i])) && (word < std::get<0>(tempNode->words[i+1]))) {
            i++;
            break;
          }
          else if (word < std::get<0>(tempNode->words[0])) break;
          else continue;
        }
        if ((tempNode->children[i])->currentNumKeys == NUMKEYS) {
          wordTup temp = split(tempNode, i);
          tempNode->words[tempNode->currentNumKeys] = temp;
          tempNode->currentNumKeys++;
          continue;
        }
        else tempNode = tempNode->children[i];
      }
    }
  }
  tempNode->words[tempNode->currentNumKeys] = wordTup(word, docs);
  sortKeys(tempNode->words, tempNode->currentNumKeys);
  tempNode->currentNumKeys++;
}

void BPlusTreeIndex::sortKeys(wordTup* wordDocTuples, int numKeys){
  wordTup temp;
  for (int i = 0; i < numKeys; i++) {
    for (int j = 0; j <= numKeys; j++) {
      if (std::get<0>(wordDocTuples[i]) > std::get<0>(wordDocTuples[j])) {
        temp = wordDocTuples[i];
        wordDocTuples[i] = wordDocTuples[j];
        wordDocTuples[j] = temp;
      }
    }
  }
}

wordTup BPlusTreeIndex::split(Node* x, int i){
  wordTup mid;
  Node* nodePMid = createLeafNode();
  if (i == -1) {
    mid = x->words[NUMKEYS/2];
    x->words[NUMKEYS/2] = wordTup("", std::vector<std::string>());
    x->currentNumKeys--;
    Node* nodePBeg = createLeafNode();
    nodePBeg->isLeaf = false;
    x->isLeaf = true;
    for (int j = (NUMKEYS/2)+1; j < NUMKEYS; j++) {
      nodePMid->words[j-(NUMKEYS/2)+1] = x->words[j];
      nodePMid->children[j-(NUMKEYS/2)+1] = x->children[j];
      nodePMid->currentNumKeys++;
      x->words[j] = wordTup("", std::vector<std::string>());
      x->currentNumKeys--;
    }
    for (int j = 0; j < FANOUT; j++) {
      x->children[j] = NULL;
    }
    nodePBeg->words[0] = mid;
    nodePBeg->children[nodePBeg->currentNumKeys] = x;
    nodePBeg->children[nodePBeg->currentNumKeys+1] = nodePMid;
    nodePBeg->currentNumKeys++;
    root = nodePBeg;
  } else {
    Node* tempNode = x->children[i];
    mid = tempNode->words[NUMKEYS/2];
    tempNode->words[NUMKEYS/2] = wordTup("", std::vector<std::string>());
    tempNode->currentNumKeys--;
    for (int j = (NUMKEYS/2)+1; j < NUMKEYS; j++) {
      nodePMid->words[j-(NUMKEYS/2)+1] = tempNode->words[j];
      nodePMid->currentNumKeys++;
      tempNode->words[j] = wordTup("", std::vector<std::string>());
      tempNode->currentNumKeys--;
    }
    x->children[i] = tempNode;
    x->children[i+1] = nodePMid;
  }
  return mid;
}

void BPlusTreeIndex::traverse(Node* p){
  std::cout << std::endl;
  int i;
  for (i = 0; i < p->currentNumKeys; i++) {
    if (p->isLeaf == false) {
      traverse(p->children[i]);
    }
    std::cout << " Word:" << std::get<0>(p->words[i]) << " Docs: ";
    std::vector<std::string> tempVec = std::get<1>(p->words[i]);
    for (int j = 0; j < tempVec.size(); j++) {
      std::cout << tempVec[j] << " ";
    }
  }
  if (p->isLeaf == false) {
    traverse(p->children[i]);
  }
  std::cout << std::endl;
}
