//
//  Resistor.cpp
//  Lab 3 The Resistor Network Program
// Author: Ashwin Santhosh
// Date: 11/10/204

#include "Resistor.h"

#include <iomanip>


//Constructor
Resistor::Resistor(string name_, double resistance_, int endpoints_[2]) {
  name = name_;
  resistance = resistance_;
  endpointNodeIDs[0] = endpoints_[0];
  endpointNodeIDs[1] = endpoints_[1];
}

//Getters and setters
string Resistor::getName() {
  return name;
}  
double Resistor::getResistance() {
  return resistance;
}   
void Resistor::setResistance(double resistance_) {
  resistance = resistance_;
}
int Resistor::getOtherEndpoint(int nodeIndex) {
  if (endpointNodeIDs[0] == nodeIndex) {
    return endpointNodeIDs[1];
  } else {
    return endpointNodeIDs[0];
  }
}

//Print function
void Resistor::print() {
  cout << std::left << std::setw(20) << name << std::right << std::setw(8)
       << std::fixed << std::setprecision(2) << resistance << " Ohms "
       << endpointNodeIDs[0] << " -> " << endpointNodeIDs[1] << endl;
}
