//
//  main.cpp
//  RedBlack
//
//  Created by Zander Nickle on 3/7/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#include <iostream>
#include <vector>
#include "rbtree-assigned.cpp"

int main(int argc, const char * argv[]) {
  
  RBTree <int, int> *myTree = new RBTree<int, int>();
  int testSize = 10000000;
  std::vector<int*> testVec(testSize);
  for (int i = 0; i < testSize; i++) {
    int addRemoveLookup = rand() % testSize; //credit to Mason,Chris
    int which = rand() % 3 + 1;
    if (which == 1) {
      int *val = new (int)(rand());
      if (myTree->insert(addRemoveLookup, val)) {
        testVec[addRemoveLookup] = val;
      }
      assert(myTree->lookup(addRemoveLookup) == testVec[addRemoveLookup]);
    } else if(which == 2) {
      int *toDelete = myTree->lookup(addRemoveLookup); // credit to Eric
      myTree->remove(addRemoveLookup);
      delete toDelete;
      testVec[addRemoveLookup] = nullptr;
    } else {
      if(myTree->lookup(addRemoveLookup) == nullptr) {
        assert(testVec[addRemoveLookup] == nullptr);
      } else {
        assert(myTree->lookup(addRemoveLookup) == testVec[addRemoveLookup]);
      }
    }
    myTree->checkRep();
  }
  
  return 0;
}
