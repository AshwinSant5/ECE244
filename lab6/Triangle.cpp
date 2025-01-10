//
//  Triangle.cpp
//  Lab5
//
//  Created by Tarek Abdelrahman on 2020-11-25.
//  Copyright © 2020 Tarek Abdelrahman.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.

//  ECE244 Student: Write the implementation of the class Rectangle here

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#include "Triangle.h"
#include "Shape.h"

Triangle::Triangle(string n, float xcent, float ycent, float x1, float y1, float x2, float y2, float x3, float y3):Shape(n, xcent, ycent) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->x3 = x3;
    this->y3 = y3;
}

Triangle::~Triangle() {
    // Nothing to do
}

// Accessors
float Triangle::getX1() const {
    return x1;
}
float Triangle::getY1() const {
    return y1;
}
float Triangle::getX2() const {
    return x2;
}
float Triangle::getY2() const {
    return y2;
}
float Triangle::getX3() const {
    return x3;
}
float Triangle::getY3() const {
    return y3;
}

// Mutators
void Triangle::setX1(float x1) {
    this->x1 = x1;
}
void Triangle::setY1(float y1) {
    this->y1 = y1;
}
void Triangle::setX2(float x2) {
    this->x2 = x2;
}
void Triangle::setY2(float y2) {
    this->y2 = y2;
}
void Triangle::setX3(float x3) {
    this->x3 = x3;
}
void Triangle::setY3(float y3) {
    this->y3 = y3;
}

void Triangle::draw() const {
  // Set floating point printing to fixed point with 2 decimals
  cout << std::fixed;
  cout << std::setprecision(2);

  // Print the information
  cout << "triangle: " << name << " "
       << x_centre << " " << y_centre
       << " " << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " " << y3 << " " << computeArea() << endl;
}

float Triangle::computeArea() const {
  float area = 0.5 * abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)));
  return (area);
}

Shape* Triangle::clone() const {
  return (new Triangle(*this));
}