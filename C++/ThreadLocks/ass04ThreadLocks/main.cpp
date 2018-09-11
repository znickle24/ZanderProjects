//
//  main.cpp
//  ass04ThreadLocks
//
//  Created by Zander Nickle on 3/6/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

int numShoppers, numSeconds;
std::vector<int> csCount;

void shoppers(int id, std::atomic_int *ticketMachine, std::atomic_int *nowServing, std::chrono::system_clock::time_point stopTime, int *criticalSectionCount, std::atomic_int *threadsWaiting) {
  (*threadsWaiting)--;
  while((*threadsWaiting) >0) {
    std::this_thread::yield();
  }
  while (std::chrono::system_clock::now() < stopTime) {
    //take a number ? is this the ticket machine?
    int mynumber;
    mynumber = (*ticketMachine)++;
    while (mynumber != *nowServing) {
      std::this_thread::yield();
    }
    criticalSectionCount[id]++; //make sure the correct thread is being counted in array
    (*nowServing)++; //make sure to initialize and reference the now serving, ticket machine and count
  }
}
int main(int argc, const char * argv[]) {
  numShoppers =  atoi(argv[1]);
  numSeconds = atoi(argv[2]);
  std::atomic_int *threadsWaiting = new std::atomic_int;
  *threadsWaiting = atoi(argv[1]);
  std::atomic_int ticketMachine;
  std::atomic_int nowServing;
  auto now = std::chrono::system_clock::now(); //current time
  auto stoppingTime = now + std::chrono::seconds(numSeconds);
  csCount.resize(numShoppers);
  std::vector<std::thread> threads;
  for (int i = 0; i < numShoppers; i++) {
    csCount[i] = 0;
    threads.push_back(std::thread(shoppers, i, &ticketMachine, &nowServing, stoppingTime, csCount.data(), threadsWaiting));
  }
  for (int i = 0; i < numShoppers; i++) {
    threads[i].join();
  }
  for (int i = 0; i < numShoppers; i++) {
    std::cout << csCount[i] << "\n";
  }
  return 0;
}
