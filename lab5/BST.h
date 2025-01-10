#ifndef _BST_
#define _BST_
#include <iostream>
#include <string>

#include "Employee.h"
using namespace std;

class BST {
 private:
  Employee* root;
  string order;  // name or ID or age

 public:
  BST(string order);  // initialize root and order
  ~BST();
  // newEmployee is a pointer to a dynamically allocated Employee. Insert it
  // according to the value of "order" of the binary search tree.
  void insert(Employee* newEmployee);

  // print the Employees in the tree according its order
  // Doesn't print anything if nothing is found
  void printInOrder();

  // search for an employee with a particular ID
  // Prints "<ID> ID does not exist" if ID doesn't exist
  Employee* searchID(int ID);

  // search for employees within the age range
  // Doesn't print anything if nothing is found
  void searchAgeRange(double lowAge, double highAge);

  // Search for employees with names having the same prefix in the firstName +
  // secondName If available, it prints them in order of their names in
  // alphabetical order
  // Doesn't print anything if nothing is found
  void autocomplete(string prefix);
};
#endif
