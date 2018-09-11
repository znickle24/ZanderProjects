//
//  main.cpp
//  simulator
//
//  Created by Zander Nickle on 2/15/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
// run with debug mode on = true or off = false
bool debug = false;
bool tellerFree = false;
int timeTellerAvailable = 0, customersServed = 0;
struct Customer {
  bool operator<(const Customer &rhs) const;
  bool isServed;
  int timeToServe;
  int totalTime;
  int timeInLine;
  int arrivalTime;
};
// struct Event {
//
//  // need to record the type of event that is occurring
//  int type;
//  Customer customer;
//};
struct BankEmployee {
  int timeStartingWithCustomer;
  bool isFree;
  Customer servingCustomer;
};
struct superMarketEmployee {
  int timeStartingWithCustomer;
  bool isFree;
  int totalWaitTime;
  Customer servingCustomer;
  std::priority_queue<Customer> isleLine;
};
//variance: (mean-individual1)^2 + mean -individual2)^2 ... /total number served
bool Customer::operator<(const Customer &rhs) const {
  if (!((arrivalTime < rhs.arrivalTime) && (arrivalTime != rhs.arrivalTime))) {
    return true;
  } else {
    return false;
  }
}

void simulateBank() {
  int time = 0;
  int customerNum = 0;
  std::priority_queue<Customer> initialBankLine;
  std::vector<BankEmployee> employees;
  std::vector<Customer> customers;
  for (int i = 0; i < 10; i++) {
    BankEmployee teller;
    teller.timeStartingWithCustomer = 0;
    teller.isFree = true;
    employees.push_back(teller);
  }
  for (int i = 0; i < 900; i++) {
    Customer customer;
    customer.isServed = false;
    customer.arrivalTime = -1;
    customer.timeToServe = -1;
    customer.timeInLine = 0;
    customers.push_back(customer);
  }
  while (time < 28800) {
    if (time % 32 == 0) {
      if (customerNum < customers.size()) {
        customers[customerNum].arrivalTime = time;
        customers[customerNum].timeToServe = rand() % (600 - 30) + 30;
        if (debug) {
          std::cout << customers[customerNum].timeToServe << "\n";
        }
        initialBankLine.push(customers[customerNum]);
        customerNum++;
      }
    }
    for (auto &employee : employees) {
      if (employee.isFree) {
        tellerFree = true;
      }
      // check for seeing if the teller has finished serving their customer
      if (!(employee.isFree) && (employee.timeStartingWithCustomer +
                                     employee.servingCustomer.timeToServe ==
                                 time)) {
        employee.isFree = true;
        employee.servingCustomer.isServed = true;
        customersServed++;
      }
      // if there is a customer in line (it's past their arrival time) and a
      // teller is free, set up that customer with the teller.  clear the
      // customer from the front of the queue, once finished, move on to next
      // teller for these checks.
      if (employee.isFree && (!(initialBankLine.empty()))) {
        employee.servingCustomer = initialBankLine.top();
        initialBankLine.pop();
        employee.servingCustomer.timeInLine =
            time - employee.servingCustomer.arrivalTime;
        employee.servingCustomer.isServed = false;
        employee.timeStartingWithCustomer = time;
        employee.isFree = false;
      }
    }
    if (tellerFree) {
      timeTellerAvailable++;
      tellerFree = false;
    }
    time++;
  }
  int timeInStore = 0, avgTimeInStore = 0;
  for (auto client : customers) {
    timeInStore += (client.timeInLine + client.timeToServe);
  }
  avgTimeInStore = timeInStore / customersServed;
  std::cout << "Time in seconds teller's available: " << timeTellerAvailable
            << "\n";
  float perTimeTellerAvailable = (timeTellerAvailable*1.0)/28800;
  std::cout << "The average time a customer was in the store today was: "
            << avgTimeInStore << "\n";
  std::cout << "The number of customers served: " << customersServed << "\n";
  std::cout << "The percent of time the teller is available is: "
            << perTimeTellerAvailable << "\n";
}
int findMinLineTime (std::vector<superMarketEmployee> employees) {
  int index = 0;
  for (int i = 1; i < employees.size(); i++) {
    if (employees[i].totalWaitTime < employees[index].totalWaitTime) {
      index = i;
    }
  }
  return index;
}
 void simulateMarket() {
   int time = 0;
   int customerNum = 0;
   std::priority_queue<Customer> initialMarketLine;
   std::vector<superMarketEmployee> employees;
   std::vector<Customer> customers;
   for (int i = 0; i < 900; i++) {
     Customer customer;
     customer.isServed = false;
     customer.arrivalTime = -1;
     customer.timeToServe = -1;
     customer.timeInLine = 0;
     customers.push_back(customer);
   }
   for (int i = 0; i < 10; i++) {
     superMarketEmployee cashier;
     cashier.timeStartingWithCustomer = 0;
     cashier.isFree = true;
     cashier.totalWaitTime = 0;
     employees.push_back(cashier);
   }
   int seed = 1;
    while (time < 28801) {
      if (time % 32 == 0) {
        if (customerNum < customers.size()) {
          customers[customerNum].arrivalTime = time;
          std::srand(std::time(nullptr));
          customers[customerNum].timeToServe = std::rand() % (600 - 30) + 30;
          seed++;
          if (debug) {
            std::cout << customers[customerNum].timeToServe << "\n";
          }
          //find the isle with the shortest line time and then add the customer that just came in to that line.
          int min = findMinLineTime(employees);
          if (employees[min].isleLine.empty()) {
            employees[min].timeStartingWithCustomer = time;
          }
          employees[min].isleLine.push(customers[customerNum]);
          employees[min].totalWaitTime += customers[customerNum].timeToServe;
          customerNum++;
        }
      }
      for (int i = 0; i < employees.size(); i++) {
        if (!employees[i].isleLine.empty()) {
          employees[i].servingCustomer = employees[i].isleLine.top();
          if (employees[i].servingCustomer.timeToServe + employees[i].timeStartingWithCustomer == time) {
            customersServed ++;
            employees[i].servingCustomer.totalTime = time - employees[i].servingCustomer.arrivalTime;
            employees[i].isleLine.pop();
            if (!employees[i].isleLine.empty()){
              employees[i].servingCustomer = employees[i].isleLine.top();
              employees[i].timeStartingWithCustomer = time;
            }
          }
          if (employees[i].isleLine.empty()) {
            employees[i].isFree = true;
          }
          if (employees[i].totalWaitTime > 0) {
            employees[i].totalWaitTime--;
          }
        }
      }
      time++;
    }
   int combinedTotal = 0, avgTimeInStore = 0;
   for (auto client: customers) {
     combinedTotal += client.totalTime;
//     std::cout << client.totalTime;
   }
   //used to calculate the time in line along with the time to serve each client, combined them in total time
//   for (auto client : customers) {
//     timeInStore += (client.timeInLine + client.timeToServe);
//   }
   avgTimeInStore = combinedTotal / customersServed;
   std::cout << "Time in seconds teller's available: " << timeTellerAvailable
   << "\n";
   float perTimeTellerAvailable = (timeTellerAvailable*1.0)/28800;
   std::cout << "The average time a customer was in the store today was: "
   << avgTimeInStore << "\n";
   std::cout << "The number of customers served: " << customersServed << "\n";
   std::cout << "The percent of time the teller is available is: "
   << perTimeTellerAvailable << "\n";
}

int main(int argc, const char *argv[]) {
//  simulateBank();
  simulateMarket();
//      const char* initialCommand = argv[1];
//    std::string initialCommandString = argv[1];
//  if (strncmp(initialCommand, "bank", 0) || strncmp(initialCommand, "supermarket", 0)) {
//          std::perror("Incorrect Queue Type");
//      }
//      if (initialCommand == NULL) {
//          std::perror("Please enter an argument of either bank or supermarket");
//      }
//      if (strncmp(initialCommand, "bank", 1)) {
//          simulateBank();
//      } else {
//          simulateMarket();
//      }
  return 0;
}
