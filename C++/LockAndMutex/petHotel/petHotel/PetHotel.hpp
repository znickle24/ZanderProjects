//
//  petHotel.hpp
//  petHotel
//
//  Created by Zander Nickle on 3/13/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#ifndef PetHotel_hpp
#define PetHotel_hpp

#include <stdio.h>
#include <thread>
#include <chrono>


class PetHotel {
public:
  int numBirds;
  int numCats;
  int numDogs;
  mutable std::mutex mutex;
  std::condition_variable noCats, noOthers;
  PetHotel(); 
  void bird();
  void cat();
  void dog();
private:
  void play() const;
};

#endif /* petHotel_hpp */
