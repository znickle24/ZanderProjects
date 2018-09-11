//
//  main.cpp
//  MakeYourOwnVector
//
//  Created by Zander Nickle on 9/11/17.
//  Copyright © 2017 Zander Nickle. All rights reserved.
//

#include <iostream>
#include "StructsAndFunctions.hpp"

using namespace std;


int main(int argc, const char * argv[]) {
    // test 1 to make sure a vector is made and to make sure that it is filled with the correct info.
    
    
    vectorInfo<int> test1 = vectorInfo<int>{5};
    test1.pushBack(1);
    test1.pushBack(2);
    test1.pushBack(3);
    
    cout << test1.getI(0) << endl;
    cout << test1.getI(1) << endl;
    cout << test1.getI(2) << endl;
    
    //test to see if the value at 3 is changed and reprint that data.
//    test1.popBack(3);
//    cout << test1.getI(3);
    
    //make sure that the value at index 1 is 2
    cout << test1.getI(1) << endl;
    
    vectorInfo<int> test2 = vectorInfo<int>{3};
    test2.pushBack(1);
    test2.pushBack(3);
    test2.pushBack(5);
    test2.pushBack(7);
    
    //testing copy method (public)
    vectorInfo<int> v1 = vectorInfo<int>{3};
    v1.pushBack(2);
    v1.pushBack(4);
    v1.pushBack(6);
    
    vectorInfo<int> v2 = v1;
    
    cout << v2.getI(2) << endl;
    cout << v2.getI(1) << endl;
    
    
    
    //testing = operator method (public)
    
    vectorInfo<int> v3 = vectorInfo<int> {25};
    
    v3.pushBack(1);
    v3.pushBack(3);
    v3.pushBack(5);
    v3.pushBack(7);
    v3.pushBack(9);
    
    vectorInfo<int> v4 = vectorInfo<int> {25};
    v4 = v3;
    cout << v4.getI(0) << endl;
    cout << v4.getI(1) << endl;
    cout << v4.getI(2) << endl;
    
    vectorInfo<int> v6 = vectorInfo<int> {4};
    v6.pushBack(3);
    v6.pushBack(1);
    v6.pushBack(8);
    v6.pushBack(2);
    
   
    v6.sortVec();
    
    cout << v6[0] << endl;
    cout << v6[1] << endl;
    cout << v6[2] << endl;
    cout << v6[3] << endl;
    
    
    return 0;
}
