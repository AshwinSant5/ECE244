//
//  main.cpp
//  Lab 3 The Resistor Network Program
//
//  Created by Nathan Hung on 2024-09-11.
//  Modified by Salma Emara on 2024-09-30
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

#define MIN_ITERATION_CHANGE 0.0001

using namespace std;

Resistor** resistors =
    nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // maximum number of nodes as set in the command line
int maxResistors = 0;  // maximum number of resistors as set in the command line
int resistorsCount = 0;  // count the number of resistors

string errorArray[10] = {
    "invalid command",                                  // 0
    "invalid argument",                                 // 1
    "negative resistance",                              // 2
    "node value is out of permitted range",             // 3
    "resistor name cannot be keyword \"all\"",          // 4
    "both terminals of resistor connect to same node",  // 5
    "too few arguments",                                // 6
};

// Function Prototypes
bool getInteger(stringstream& ss, double& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);

void handleMaxVal(stringstream& ss);
void handleInsertR(stringstream& ss);
void handleModifyR(stringstream& ss);
void handlePrintR(stringstream& ss);
void handleDeleteR(stringstream& ss);
void handleSetV(stringstream& ss);

void handleSolve();

int main() {
  string line;
  cout << ">>> ";
  cout.flush();
  // TODO: Implement the main function here
  //while the input is not "EOF"
  while (getline(cin, line)) {
    //take the input and put it into a stringstream
    stringstream ss(line);
    //take the first word of the input as command
    string command;
    ss >> command;
    //conditions specifying command
    if (command == "maxVal") {
      handleMaxVal(ss);
    } else if (command == "insertR") {
      handleInsertR(ss);
    } else if (command == "modifyR") {
      handleModifyR(ss);
    } else if (command == "printR") {
      handlePrintR(ss);
    } else if (command == "deleteR") {
      handleDeleteR(ss);
    } else if (command == "setV") {
      handleSetV(ss);
    } else if (command == "solve") {
      handleSolve();
    } else {
      cout << "Error: " << errorArray[0] << endl; //if not a valid command or empty input
    }
    //print the prompt
    cout << ">>> ";
    //flush the output
    cout.flush();
  }
  //end program
  return 0;
}

// TODO: Implement functions here

// Function to get integer from stringstream
bool getInteger(stringstream& ss, int& x) {
  string token;
  if (!(ss >> token)) {
    return false;  // No token to read
  }
  char* endptr;
  long val = strtol(token.c_str(), &endptr, 10);
  if (*endptr != '\0') {
    // Extra characters found; invalid integer
    return false;
  }
  x = static_cast<int>(val);
  return true;
}

bool getDouble(stringstream& ss, double& x) {
  string token;
  if (!(ss >> token)) {
    return false;  // No token was read; input failure
  }
  char* endptr;
  x = strtod(token.c_str(), &endptr);
  if (*endptr != '\0') {
    // Extra characters were found after parsing the number
    return false;
  }
  return true;  // Successful conversion
}

// Function to get string from stringstream
bool getString(stringstream& ss, string& s) {
  ss >> s;
  if (ss.fail()) {
    ss.clear();
    return false;
  }
  return true;
}

// Function to check if the line has ended
bool lineEnd(stringstream& ss) {
  ss >> ws;
  return ss.peek() != EOF;
}

// Function to handle maxVal command
void handleMaxVal(stringstream& ss) {
  int maxNumNodes, maxNumResistors;

  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;  // Error: too few arguments
    return;
  }
  // Error check for invalid arguments
  if (!getInteger(ss, maxNumNodes)) {
    cout << "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }
  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;  // Error: too few arguments
    return;
  }
  if (!getInteger(ss, maxNumResistors)) {
    cout <<  "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }

  // Error check for negative values
  if (maxNumNodes <= 0 || maxNumResistors <= 0) {
    cout <<  "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }

  // Delete existing resistor objects and reset resistorsCount
  if (resistors != nullptr) {
    for (int i = 0; i < resistorsCount; ++i) {
      delete resistors[i];
      resistors[i] = nullptr;
    }
    delete[] resistors;
    resistors = nullptr;
  }
  resistorsCount = 0;

  // Delete existing nodes array
  if (nodes != nullptr) {
    delete[] nodes;
    nodes = nullptr;
  }

  // Update maxNodeNumber, maxResistors and resistors count
  maxNodeNumber = maxNumNodes;
  maxResistors = maxNumResistors;
  resistorsCount = 0;

  // Allocate new arrays with the new values
  resistors = new Resistor*[maxResistors];
  for (int i = 0; i < maxResistors; ++i) {
    resistors[i] = nullptr;
  }

  nodes = new Node[maxNodeNumber];

  // Print the new network
  cout << "New network: max node number is " << maxNodeNumber
       << "; max resistors is " << maxResistors << endl;
}

//NOTE: MAYBE CHECK IF THE RESISTOR BEING ADDED TO A NODE IS ALREADY EXCEEDING MAX_RESISTORS_PER_NODE
void handleInsertR(stringstream& ss) {
  string resistorID;
  double resistance;
  int endpoints[2];

  // Check for too few arguments before reading resistorID
  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;  // Error: too few arguments
    return;
  }
  // Read resistorID
  if (!getString(ss, resistorID)) {
    cout << "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }

  // Check for "all" keyword in resistorID
  if (resistorID == "all") {
    cout << "Error: " << errorArray[4]
         << endl;  // Error: resistor name cannot be keyword "all"
    return;
  }
  
  // Check if resistor name already exists
  for (int i = 0; i < resistorsCount; i++) {
    if (resistors[i]->getName() == resistorID) {
      cout << "Error: resistor " << resistorID << " already exists" << endl;
      return;
    }
  }

  // Check for too few arguments before reading resistance
  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;  // Error: too few arguments
    return;
  }
  // Read resistance
  if (!getDouble(ss, resistance)) {
    cout << "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }

  // Check for negative resistance
  if (resistance < 0) {
    cout << "Error: " << errorArray[2] << endl;  // Error: negative resistance
    return;
  }

  // Check for too few arguments before reading first endpoint
  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;  // Error: too few arguments
    return;
  }
  // Read first endpoint
  if (!getInteger(ss, endpoints[0])) {
    cout << "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }

  // Check if first endpoint is in valid range
  if (endpoints[0] < 1 || endpoints[0] > maxNodeNumber) {
    cout << "Error: " << errorArray[3] << " " << 1 << "-" << maxNodeNumber << endl;  // Error: node value out of permitted range
    return;
  }

  // Check for too few arguments before reading second endpoint
  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;  // Error: too few arguments
    return;
  }
  // Read second endpoint
  if (!getInteger(ss, endpoints[1])) {
    cout << "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }

  // Check if second endpoint is in valid range
  if (endpoints[1] < 1 || endpoints[1] > maxNodeNumber) {
    cout << "Error: " << errorArray[3] << " " << 1 << "-" << maxNodeNumber << endl;  // Error: node value out of permitted range
    return;
  }

  // Check if both terminals connect to the same node
  if (endpoints[0] == endpoints[1]) {
    cout << "Error: " << errorArray[5] << endl;  // Error: terminals connect to same node
    return;
  }


  // Check if resistor array is full
  if (resistorsCount >= maxResistors) {
    cout << "Error: resistor array is full" << endl;
    return;
  }

  // Adjust endpoints to zero-based indices for internal storage
  int endpointIndices[2] = {endpoints[0] - 1, endpoints[1] - 1};

  // Check if nodes can accept more resistors
  if (!nodes[endpointIndices[0]].canAddResistor()) {
    cout << "Error: node " << endpoints[0] << " cannot add more resistors" << endl;
    return;
  }
  if (!nodes[endpointIndices[1]].canAddResistor()) {
    cout << "Error: node " << endpoints[1] << " cannot add more resistors" << endl;
    return;
  }

  // Create the resistor and add it to the resistor array
  resistors[resistorsCount] = new Resistor(resistorID, resistance, endpoints);

  // Add resistor index to the nodes
  nodes[endpointIndices[0]].addResistor(resistorsCount);
  nodes[endpointIndices[1]].addResistor(resistorsCount);

  resistorsCount++;

  // Output success message
  cout << "Inserted: resistor " << resistorID << " " << fixed << setprecision(2)
       << resistance << " Ohms " << endpoints[0] << " -> " << endpoints[1]
       << endl;
}

void handleModifyR(stringstream& ss) {
  string resistorID;
  double resistance;

  // Check for too few arguments before reading resistorID
  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }
  // Read resistorID
  if (!getString(ss, resistorID)) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  // Check if the resistor exists
  bool found = false;
  for(int i = 0; i < resistorsCount; i++) {
    if(resistors[i]->getName() == resistorID) {
      found = true;
      break;
    }
  }
  if (!found) { //print error message if the resistor does not exist
    cout << "Error: resistor " << resistorID << " not found" << endl;
    return;
  }

  // Check for too few arguments before reading resistance
  if (!lineEnd(ss)) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }
  // Read resistance
  if (!getDouble(ss, resistance)) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  // Check for negative resistance
  if (resistance < 0) {
    cout << "Error: " << errorArray[2] << endl;
    return;
  }

  //check if the resistor exists
  for (int i = 0; i < resistorsCount; i++) {
    if (resistors[i]->getName() == resistorID) {
      double oldResistance = resistors[i]->getResistance();
      resistors[i]->setResistance(resistance);
      cout << "Modified: resistor " << resistorID << " from " << oldResistance <<  " Ohms to " << resistance << " Ohms" << endl;
      return;
    }
  }
  cout << "Error: resistor " << resistorID << " not found" << endl;
}

void handlePrintR(stringstream& ss) {
  string resistorID;
  //error check for invalid arguments
  if (!getString(ss, resistorID)) {
    cout <<  "Error: " << errorArray[1] << endl;
    return;
  }
  //error check for too few arguments
  if (ss.fail() && ss.eof()) {
    cout <<  "Error: " << errorArray[6] << endl;
    return;
  }
  //check if the resistor exists
  for (int i = 0; i < resistorsCount; i++) {
    if (resistors[i]->getName() == resistorID) {
      cout << "Print:" << endl;
      resistors[i]->print();
      return;
    }
  }
  cout << "Error: resistor " << resistorID << " not found" << endl;
}

  // NOTE: reduce node count for the nodes that the resistor is connected to
void handleDeleteR(stringstream& ss) {
  string resistorID;
  // Check for too few arguments
  if (ss.peek() == EOF) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }
  //error check for invalid arguments
  if (!getString(ss, resistorID)) {
    cout <<  "Error: " << errorArray[1] << endl;
    return;
  }
  //handle deleting all resistors
  if (resistorID == "all") {
    //delete all resistors
    for (int i = 0; i < resistorsCount; i++) {
      delete resistors[i];
      resistors[i] = nullptr;
    }
    delete [] resistors;
    resistors = nullptr;
    resistors = new Resistor*[maxResistors];
    for (int i = 0; i < maxResistors; ++i) {
      resistors[i] = nullptr;
    }
    resistorsCount = 0;

    delete [] nodes;
    nodes = nullptr;
    nodes = new Node[maxNodeNumber];

    cout << "Deleted: all resistors" << endl;
    return;
  }
  cout << "Error: " << errorArray[1] << endl;

  // // check if the resistor exists
  // for (int i = 0; i < resistorsCount; i++) {
  //   if (resistors[i]->getName() == resistorID) {     
  //     //delete the resistor and move the last resistor to the deleted resistor's position
  //     delete resistors[i];
  //     resistors[i] = resistors[resistorsCount - 1];
  //     resistorsCount--;
  //     //print the deleted resistor
  //     cout << "Deleted: resistor " << resistorID << endl;
  //     return;
  //   }
  // }
  // cout << "Error: resistor " << resistorID << " not found" << endl;
}

void handleSetV(stringstream& ss) {
  int nodeID;
  double voltage;

  // Check for too few arguments
  if (ss.peek() == EOF) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }
  // Error check for invalid argumentss
  if (!getInteger(ss, nodeID)) {
    cout <<  "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }
  // Check for too few arguments
  if (ss.peek() == EOF) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }
  if (!getDouble(ss, voltage)) {
    cout <<  "Error: " << errorArray[1] << endl;  // Error: invalid argument
    return;
  }

  // Error check for node value out of permitted range (1 to maxNodeNumber)
  if (nodeID < 1 || nodeID > maxNodeNumber) {
    cout <<  "Error: " << errorArray[3]
         << endl;  // Error: node value is out of permitted range
    return;
  }

  // Adjust nodeID to zero-based index for internal array
  int nodeIndex = nodeID - 1;

  // Set the voltage of the node
  nodes[nodeIndex].setVoltage(voltage);

  // Output success message
  cout << "Set: node " << nodeID << " to " << fixed << setprecision(2)
       << voltage << " Volts" << endl;
}

//handle solve
void handleSolve() {
  //Initialize the voltage of all nodes without a specified(setV) voltage to 0.
  for (int i = 0; i < maxNodeNumber; i++) {
    if (!nodes[i].isVoltageSet()) {
      nodes[i].setVoltageInternally(0);
    }
  }
  //store the old voltages of all nodes
  double oldVoltages[maxNodeNumber] = {0};
  for (int i = 0; i < maxNodeNumber; i++) {
    oldVoltages[i] = nodes[i].getVoltage();
  }

  //store the new voltages of all nodes
  double newVoltages[maxNodeNumber] = {0};
  //initialize the new voltages of all nodes to the old voltages
  std::copy(oldVoltages, oldVoltages + maxNodeNumber, newVoltages);
  //boolean to check if the voltage at each node has changed by more than MIN_ITERATION_CHANGE
  bool changed = false;
  // while some node’s voltage has changed by more than MIN_ITERATION_CHANGE
  do {
    //loop through each node
    for (int i = 0; i < maxNodeNumber; i++) {
      if (nodes[i].isVoltageSet() || nodes[i].getNumRes() == 0) {
        continue;
      }

      else {
      //calculate the new voltage of each node
      double denom = 0;
      double vOverR = 0;
      //loop through number of connected resistors
      for (int j = 0; j < nodes[i].getNumRes(); j++) {
        //get the resistor index
        int resistorIndex = nodes[i].getResIDArray()[j];
        //get the other node that the resistor is connected to
        int otherNode = resistors[resistorIndex]->getOtherEndpoint(i + 1) - 1;
        //calculate the sum of the voltages of the connected nodes
        //sum += (oldVoltages[i] - oldVoltages[otherNode]) / resistors[resistorIndex]->getResistance();
        denom += 1 / resistors[resistorIndex]->getResistance();
        vOverR += oldVoltages[otherNode] / resistors[resistorIndex]->getResistance();
      }
      newVoltages[i] = 1/denom * vOverR;
      }
    }
    //check to see if voltage at each node has changed by more than MIN_ITERATION_CHANGE
    changed = false;
    for(int i = 0; i < maxNodeNumber; i++) {
      if (abs(oldVoltages[i] - newVoltages[i]) > MIN_ITERATION_CHANGE) {
        changed = true;
        break;
      }
    }
    //update the old voltages to the new voltages
    std::copy(newVoltages, newVoltages + maxNodeNumber, oldVoltages);
    if (!changed) {
      cout << "Solve:" << endl;
      for (int i = 0; i < maxNodeNumber; i++) {
        if (nodes[i].getNumRes() == 0) {
          continue;
        }
        nodes[i].setVoltage(newVoltages[i]);
        cout << "  Node " << i + 1 << ": " << fixed << setprecision(2) << newVoltages[i] << " V" << endl;
      }
    }
  } while (changed);
} 
