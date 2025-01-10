#include "RegisterList.h"

#include <iostream>

#include "Register.h"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
  Register* temp = head;
  while (temp != nullptr) {
    Register* next = temp->get_next();
    delete temp;
    temp = next;
  }
  head = nullptr;
}

Register* RegisterList::get_head() {  
  return head;
}

int RegisterList::get_size() { 
  // return number of registers 
  return size;
}

Register* RegisterList::get_min_items_register() {
  if (head == nullptr) {
    return nullptr;
  }

  Register* minItemsRegister = head;
  Register* temp = head->get_next();

  while (temp != nullptr) {
    if (temp->get_queue_list()->get_items() <
        minItemsRegister->get_queue_list()->get_items()) {
      minItemsRegister = temp;
    }
    temp = temp->get_next();
  }

  return minItemsRegister;
}

Register* RegisterList::get_free_register() {
  // return the register with no customers
  // if all registers are occupied, return nullptr
  Register* temp = head;
  while (temp != nullptr) {
    if (temp->get_queue_list()->get_head() == nullptr) {
      return temp;
    }
    temp = temp->get_next();
  }
  return nullptr;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 
  Register* temp = head;
  if (temp == nullptr) {
    head = newRegister;
    size++;
    return;
  }

  while (temp->get_next() != nullptr) {
    temp = temp->get_next();
  }
  temp->set_next(newRegister);
  size++;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise

  if(size == 0) {
    return false;

  }
  Register* temp = head;
  for(int i = 0; i < size; i++) {
    if(temp->get_ID() == ID) {
      return true;
    }
    temp = temp->get_next();
  }
  return false;
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found

  if(size == 0) {
    return nullptr;
  }

  Register* temp = head;
  Register* prev = nullptr;

  //loop through the registers
  for(int i = 0; i < size; i++) {
    if(temp->get_ID() == ID) {  //if the register is found
      if(prev == nullptr) { //if the register is the head
        head = temp->get_next();
      } else { //if the register is not the head
        prev->set_next(temp->get_next());
      }
      //reduce size and return the register
      size--;
      return temp;
    }
    //move to the next register
    prev = temp;
    temp = temp->get_next();
  }
  return nullptr;
}

//function returns register with minimum departure time (returns nullptr if no customer can depart in the current time)
Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr

  // if register list is empty
  if(size == 0) {
    return nullptr;
  }

  //initialize a moving index and the register with the minimum departure time
  Register* temp = head;
  Register* minDepartTimeRegister = nullptr;
  double minDepartTime = -1; //initialize minimum time of departure

  while (temp != nullptr) {
    // Get the departure time of the next customer in the queue
    double departTime = temp->calculateDepartTime();

    // Check if the customer can depart now
    if (departTime != -1 && departTime <= expTimeElapsed) {
      // If this is the first customer found or has an earlier departure time
      if (minDepartTime == -1 || departTime < minDepartTime) {
        minDepartTime = departTime;
        minDepartTimeRegister = temp;
      }
    }
    temp = temp->get_next();
  }

  // Return the register with the earliest departure time among customers who can depart now
  return minDepartTimeRegister;
}