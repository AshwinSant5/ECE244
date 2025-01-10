#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }
  // You have to make sure all dynamically allocated memory is freed 
  // before return 0
  delete registerList;

  //calculate average wait time & max wait time
  double waitTime = 0;   // total wait time of all customers
  double deviationSum = 0;  // standard deviation of wait time
  int numCustomers = 0;  // number of customers
  double averageWaitTime = 0;    // average wait time
  double max = 0; // max wait time

  Customer *temp = doneList->get_head();

  //loop through the done list
  while (temp != nullptr) {
    //calculate the wait time of each customer
    double customerWaitTime = temp->get_departureTime() - temp->get_arrivalTime();
    waitTime += customerWaitTime;
    //update the max wait time
    if (customerWaitTime > max) {
      max = customerWaitTime;
    }
    //move to the next customer
    temp = temp->get_next();
    numCustomers++;
  }
  //calculate the average wait time
  averageWaitTime = waitTime / (double)numCustomers;

  //calculate standard deviation
  temp = doneList->get_head();
  //loop through the done list
  while (temp != nullptr) {
    //calculate the deviation of each customer
    deviationSum += pow((temp->get_departureTime() - temp->get_arrivalTime()) - averageWaitTime, 2);
    //move to the next customer
    temp = temp->get_next();
  }
  //calculate the standard deviation
  double standardDeviation = sqrt(deviationSum / (double)numCustomers);

  //display statistics
  cout << "\nFinished at time " << expTimeElapsed << endl;
  cout << "Statistics:" << endl;
  cout << "Maximum wait time: " << max << endl;
  cout << "Average wait time: " << averageWaitTime << endl;
  cout << "Standard Deviation of wait time: " << standardDeviation << endl;

  //free memory
  delete doneList;
  delete singleQueue;

  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }

  // Update the system time
  expTimeElapsed += timeElapsed;

  // Process any customer departures that should occur at this update
  Register *reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed); //get the register with the minimum departure time
  //loop until no customer can depart
  while (reg != nullptr) {
    //calculate the departure time of the customer
    double departureTime = reg->calculateDepartTime();
    //remove the customer from the queue and add to done list
    reg->departCustomer(doneList);
    //print the departure message
    cout << "Departed a customer at register ID " << reg->get_ID() << " at " << departureTime << endl;

    // In single mode, assign a waiting customer to the now-free register
    if (mode == "single") {
      // If there are customers in the single queue
      if (singleQueue->get_head() != nullptr) {
        // Assign the customer to the register
        Customer *customer = singleQueue->dequeue();
        // Add the customer to the register's queue
        reg->get_queue_list()->enqueue(customer);
        // Print message
        cout << "Queued a customer with free register " << reg->get_ID() << endl;
      }
    }
    //get the register with the minimum departure time
    reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }

  // Create the new customer
  Customer *newCustomer = new Customer(expTimeElapsed, items);
  cout << "A customer entered" << endl;

  if (mode == "single") {
    // Add the customer to the single queue
    singleQueue->enqueue(newCustomer);

    //get free register
    Register *freeRegister = registerList->get_free_register();
    //if there is a free register
    if (freeRegister != nullptr) {
      //dequeue the customer from the single queue and add to the register
      Customer *customer = singleQueue->dequeue();
      freeRegister->get_queue_list()->enqueue(customer);
      //print the message
      cout << "Queued a customer with free register " << freeRegister->get_ID() << endl;
    } 
    else {
      cout << "No free registers" << endl;
    }
  }

  else if (mode == "multiple") {
    // Existing code for multiple mode
    Register *reg2 = registerList->get_min_items_register();
    //if there iis no register open
    if (reg2 == nullptr) {
      cout << "No registers are open. Cannot add customer." << endl;
      //if there are no registers open, delete the customer to prevent memory leak
      delete newCustomer; 
      return;
    }
    //add the customer to the register with the minimum number of items
    reg2->get_queue_list()->enqueue(newCustomer);
    //print the message
    cout << "Queued a customer with quickest register " << reg2->get_ID() << endl;
  }
}

void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}


void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register
  
  // Check if the register is already open
  if(registerList->foundRegister(ID)) {
    // Print an error message
    cout << "Error: register " << ID << " is already open" << endl;
    return;
  }

  // Update the system time
  expTimeElapsed += timeElapsed;

  // Process any customer departures that should occur in the current update
  Register *reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed); //get the register with the minimum departure time
  //loop until no customer can depart
  while (reg != nullptr) {
    //calculate the departure time of the customer
    double departureTime = reg->calculateDepartTime();
    reg->departCustomer(doneList); //remove the customer from the queue and add to done list
    //print the departure message
    cout << "Departed a customer at register ID " << reg->get_ID() << " at " << departureTime << endl;

    // In single queue mode, assign a waiting customer to the now-free register
    if(mode == "single") {
      // If there are customers in the single queue
      if(singleQueue->get_head() != nullptr) {
        // Assign the customer to the register
        reg->get_queue_list()->enqueue(singleQueue->dequeue());
        cout << "Queued a customer with free register " << reg->get_ID() << endl;
      }
    }
    //get the register with the minimum departure time
    reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }

  // Open the register
  Register *newRegister = new Register(ID, secPerItem, setupTime, expTimeElapsed);
  // Add the register to the register list
  registerList->enqueue(newRegister);
  cout << "Opened register " << ID << endl;

  //if the mode is single
  if(mode == "single") {
    // If there are customers in the single queue
    if(singleQueue->get_head() != nullptr) {
      //assign the customer to the register
      newRegister->get_queue_list()->enqueue(singleQueue->dequeue());
      cout << "Queued a customer with free register " << ID << endl;
    }
  }
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // Convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Update the system time
  expTimeElapsed += timeElapsed;

  // Process any customer departures for the current update
  Register *reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed); //get the register with the minimum departure time
  //loop until no customer can depart
  while (reg != nullptr) {
    //calculate the departure time of the customer
    double departureTime = reg->calculateDepartTime();
    //remove the customer from the queue and add to done list
    if (departureTime <= expTimeElapsed) {
      // Remove the customer from the queue and add to done list
      reg->departCustomer(doneList);
      cout << "Departed a customer at register ID " << reg->get_ID() << " at " << departureTime << endl;

      // In single queue mode, assign a waiting customer to the free register
      if (mode == "single") {
        // If there are customers in the single queue
        if (singleQueue->get_head() != nullptr) {
          // Assign the customer to the register
          Customer *customer = singleQueue->dequeue();
          // Add the customer to the register's queue
          reg->get_queue_list()->enqueue(customer);
          // Print message
          cout << "Queued a customer with free register " << reg->get_ID() << endl;
        }
      }
    } else {
      // No more departures to process
      break;
    }
    // Get the register with the minimum departure time
    reg = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  }

  // Close the register
  Register *regToClose = registerList->dequeue(ID);
  // Check if the register isn't open
  if (regToClose == nullptr) {
    cout << "Error: register " << ID << " is not open" << endl;
    return;
  }

  // remaining customers are not moved back to the single queue remaining customers are lost when the register closes

  //close the register
  cout << "Closed register " << ID << endl;
  delete regToClose;

  // In single queue mode, assign waiting customers to other free registers
  if (mode == "single") {
    // Assign waiting customers to other free registers
    while (singleQueue->get_head() != nullptr) {
      // Get a free register
      Register *freeRegister = registerList->get_free_register();
      // If there is a free register
      if (freeRegister != nullptr) {
        // Dequeue the customer from the single queue and add to the register
        Customer *customer = singleQueue->dequeue();
        freeRegister->get_queue_list()->enqueue(customer);
        // Print the message
        cout << "Queued a customer with free register " << freeRegister->get_ID() << endl;
      } 
      else {
        break;
      }
    }
  }
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}
