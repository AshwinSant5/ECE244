#ifndef _EMPLOYEE_
#define _EMPLOYEE_

#include <iostream>
#include <string>
using namespace std;

class Employee {
 private:
  string firstName;
  string lastName;
  int ID;
  double age;
  double salary;
  Employee* left;
  Employee* right;

 public:
  Employee(string firstName_, string lastName_, int ID_, double age_,
           double salary_);
  ~Employee();
  int getID();
  string getName();
  double getAge();
  double getSalary();
  Employee* getLeft();
  Employee* getRight();

  void setLeft(Employee* l);
  void setRight(Employee* r);
  void print();
};
#endif
