//
//  Node.cpp
//  Lab 3 The Resistor Network Program
// Author: Ashwin Santhosh
// Date: 11/10/204
#include "Node.h"

Node::Node() {
    numRes = 0;
    voltage = 0;
    voltageIsSet = false;
}

bool Node::canAddResistor() {
    if (numRes < MAX_RESISTORS_PER_NODE) {
        return true;
    }
    return false;
}

void Node::addResistor(int rIndex) {
    resIDArray[numRes] = rIndex;
    numRes++;
}

double Node::getVoltage() {
    return voltage;
}

void Node::setVoltage(double voltage_) {
    voltage = voltage_;
    voltageIsSet = true;
}

void Node::setVoltageInternally(double voltage_) {
    voltage = voltage_;
}

bool Node::isVoltageSet() {
    return voltageIsSet;
}

int Node::getNumRes() {
    return numRes;
}

int* Node::getResIDArray() {
    return resIDArray;
}
