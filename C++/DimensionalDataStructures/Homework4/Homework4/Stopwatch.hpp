//
//  Stopwatch.hpp
//  Homework4
//
//  Created by Zander Nickle on 6/12/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#ifndef Stopwatch_h
#define Stopwatch_h

#pragma once
#include <chrono>


/*
 Simple timing class.  Gives the number of seconds as a double
 Stopwatch sw;
 sw.start();
 doTheThing();
 double elapsedSeconds = sw.stop();
 The stopwatch keeps running.  Call start() again to reset it.
 */

class Stopwatch{
  
public:
  
  void start(){
    startTime = std::chrono::high_resolution_clock::now();
  }
  
  double stop(){
    return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startTime).count();
  }
private:
  
  std::chrono::high_resolution_clock::time_point startTime;
};

#endif /* Stopwatch_h */
