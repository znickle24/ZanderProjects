//
//  StructsAndFunctions.hpp
//  MakeYourOwnVector
//
//  Created by Zander Nickle on 9/11/17.
//  Copyright © 2017 Zander Nickle. All rights reserved.
//

#ifndef StructsAndFunctions_hpp
#define StructsAndFunctions_hpp

#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

template <typename t>
class vectorInfo {
    t * data;
    int size;
    int capacity;
    
public:
    vectorInfo (int initialCapacity) ;
    t freeVector ();
    void popBack ( int value);
    t getI (int index) const ;
    t set (int index, t newValue);
    void growData ();
    void pushBack (int value);
    int getSize () const;
    int getCapacity () const;
    vectorInfo<t> (const vectorInfo<t>& rhs);
    vectorInfo<t>& operator = (const vectorInfo<t>& rhs);
    t operator [] (int index) const;
    t& operator[](int index);
    bool operator == (const vectorInfo<t>& rhs) ;
    bool operator != (const vectorInfo<t>& rhs) ;
    bool operator < (const vectorInfo<t>& rhs) ;
    bool operator >= (const vectorInfo<t>& rhs) ;
    bool operator > (const vectorInfo<t>& rhs) ;
    t indexOfMin (const vectorInfo<t>& newVec, t startVal);
    void sortVec ();
};

//creating templates for each function

template <typename t>
vectorInfo<t>::vectorInfo (int initialCapacity) {
    t * pointer = new t [initialCapacity];
    data = pointer;
    size = 0;
    capacity = initialCapacity;
}

template <typename t>

t vectorInfo<t>:: freeVector () {
    delete [] data;
    data = nullptr;
}

template <typename t>

void vectorInfo<t>:: popBack (int value) {
    if (size > 0) {
        size -= value;
    }
}

template <typename t>

t vectorInfo<t>:: getI (int index) const {
    return data[index];
}

template <typename t>
t vectorInfo<t>:: set (int index, t newValue) {
    data[index] = newValue;
}


template <typename t>
void vectorInfo<t>:: growData () {
    t* oldPointer = data;
    capacity = capacity *2;
    data = new int [capacity];
    for (int i = 0; i < size; i ++) {
        data[i] = oldPointer[i];
    }
    delete [] oldPointer;
    oldPointer = nullptr;
    
}

template <typename t>
void vectorInfo<t>:: pushBack (int value) {
    if (capacity > size) {
        size += 1;
        data [size -1] = value;
    } else {
        
        growData();
        
        size += 1;
        data [size -1] = value;
    }
    
}

template <typename t>
int vectorInfo<t>:: getSize() const {
    return (size - 1);
}

template <typename t>
int vectorInfo<t>:: getCapacity() const {
    return (capacity - 1);
}

template <typename t>
vectorInfo<t>::vectorInfo (const vectorInfo<t>& rhs){
    size = rhs.size;
    capacity = rhs.capacity;
    data = new int [capacity];
    
    for (int i = 0; i < rhs.size; i ++) {
        data[i] = rhs.data[i];
    }
}

template <typename t>
vectorInfo<t>& vectorInfo<t>:: operator = (const vectorInfo<t>& rhs) {
    //allocate new resources
    
    size = rhs.size;
    capacity = rhs.capacity;
    t* oldPointer = data;
    data = new int [capacity];
    
    
    // copy piece
    
    for (int i = 0; i < rhs.size; i ++) {
        data[i] = rhs.data[i];
    }
    
    //delete the old array
    delete [] oldPointer;
    oldPointer = nullptr;
    
    return * this ;
}


template <typename t>
t vectorInfo<t>:: operator [] (int index) const {
    return data[index];
}

template <typename t>
t& vectorInfo<t>:: operator[](int index) {
    return data [index];
}

template <typename t>
bool vectorInfo<t>:: operator == (const vectorInfo<t>& rhs) {
    if (size == rhs.size ()) {
        for (int i = 0 ; i < size(); i++) {
            if (data[i] != rhs.data[i]) {
                return false;
            }
        }
        return true;
    }
}

template <typename t>
bool vectorInfo<t>:: operator != (const vectorInfo<t>& rhs) {
    if (!(*this == rhs)) {
        return true;
    }
}

template <typename t>
bool vectorInfo<t>:: operator < (const vectorInfo<t>& rhs) {
        for (int i = 0; i < size(); i++) {
            if (data[i] > rhs.data[i]){
                return false;
            }
        }
        return true;
}

template <typename t>
bool vectorInfo<t>:: operator >= (const vectorInfo<t>& rhs) {
    if (!(*this < rhs)) {
        return true;
    }
}

template <typename t>
bool vectorInfo<t>:: operator > (const vectorInfo<t>& rhs) {
    if (!(*this < rhs) && !(*this == rhs)){
        return true;
    }
}

template <typename t>
t vectorInfo<t>:: indexOfMin (const vectorInfo<t>& newVec, t startVal) {
    int minPos = newVec.data[0];
    for (int i = 0; i < this->size; i++) {
        if (data[i] < minPos) {
            minPos = i;
        }
    } return minPos ;
}

template <typename t>
void vectorInfo<t>:: sortVec () {
    for (int i = 0; i < this->size; i++) {
        int minCard = indexOfMin(*this, i);
        t temp = data[minCard];
        data[minCard] = data[i];
        data[i]=temp;
    }
}

#endif /* StructsAndFunctions_hpp */
