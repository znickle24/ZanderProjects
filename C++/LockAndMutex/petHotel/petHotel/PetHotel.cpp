//
//  petHotel.cpp
//  petHotel
//
//  Created by Zander Nickle on 3/13/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#include "PetHotel.hpp"
#include <thread>
#include <chrono>
#include <cassert>
#include <iostream>
PetHotel::PetHotel(): numBirds(0), numCats(0), numDogs(0) {}


void PetHotel::bird() {
  {
    std::unique_lock<std::mutex> lock(mutex);
    while (numCats != 0) {
      noOthers.wait(lock);
    }
    assert(numCats == 0);
    numBirds++;
  }
  play();
  {
    std::unique_lock<std::mutex> lock(mutex);
    numBirds--;
    if (numDogs + numBirds == 0) {
      noCats.notify_all();
    }
  }
}
void PetHotel::cat() {
  {
    std::unique_lock<std::mutex> lock(mutex);
    while (numBirds !=0 || numDogs != 0) {
      noCats.wait(lock);
    }
    assert(numBirds == 0 && numDogs == 0);
    numCats++;
  }
  play();
  {
    std::unique_lock<std::mutex> lock(mutex);
    numCats--;
    if (numCats == 0) {
      noOthers.notify_all();
    }
  }
}
void PetHotel::dog() {
  {
    std::unique_lock<std::mutex> lock(mutex);
    while (numCats != 0) {
      noOthers.wait(lock);
    }
    assert(numCats == 0);
    numDogs++;
  }
  play();
  {
    std::unique_lock<std::mutex> lock(mutex);
    numDogs--;
    if (numDogs + numBirds == 0) {
      noCats.notify_all();
    }
  }
}
