
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>

Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register() {
  delete queue;
}

QueueList* Register::get_queue_list() {
  return queue;
}

Register* Register::get_next() {
  return next;
}

int Register::get_ID() { 
  return ID;
}

double Register::get_secPerItem() { 
  return secPerItem;
}

double Register::get_overheadPerCustomer() { 
  return overheadPerCustomer;
}

double Register::get_availableTime() { 
  return availableTime;
}

void Register::set_next(Register* nextRegister) { 
  next = nextRegister;
}


void Register::set_availableTime(double availableSince) {
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue

  Customer* customer = queue->get_head();
  if (customer == nullptr) {
    return -1;  // No customer to process
  }

  double departureTime;
  // Calculate processing time
  double processingTime = secPerItem * customer->get_numOfItems() + overheadPerCustomer;

  // Calculate departure time
  if (customer->get_arrivalTime() >= availableTime) {
    // Customer arrived after the register became available
    departureTime = customer->get_arrivalTime() + processingTime;
  } else {
    // Customer arrived before the register became available
    departureTime = availableTime + processingTime;
  }

  // Set the customer's departure time
  customer->set_departureTime(departureTime);

  return departureTime;
}

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, set last dequeue time, add to doneList,
  // update availableTime of the register
  
  //calculate the departure time of the customer
  double departure_time = calculateDepartTime();

  //if there is no customer in the queue
  if(departure_time == -1) {
    return;
  }

  //dequeue the customer
  Customer* customer = queue->dequeue();

  if(customer == nullptr) {
    return;
  }

  //set the departure time of the customer
  customer->set_departureTime(departure_time);
  //add the customer to the done list
  doneList->enqueue(customer);
  //update the available time of the register
  availableTime = customer->get_departureTime();
}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
