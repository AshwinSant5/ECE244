#include "Employee.h"

#include <iostream>
#include <string>
using namespace std;

Employee::Employee(string firstName_, string lastName_, int ID_, double age_,
                   double salary_) {
  firstName = firstName_;
  lastName = lastName_;
  ID = ID_;
  age = age_;
  salary = salary_;

  left = right = NULL;
}
Employee::~Employee() {
  delete left;
  delete right;
}
int Employee::getID() { return ID; }
string Employee::getName() { return firstName + lastName; }
double Employee::getAge() { return age; }
double Employee::getSalary() { return salary; }

Employee* Employee::getLeft() { return left; }
Employee* Employee::getRight() { return right; }

void Employee::setLeft(Employee* l) { left = l; }
void Employee::setRight(Employee* r) { right = r; }

void Employee::print() {
  cout << "Name: " << firstName + " " + lastName << endl;
  cout << "ID: " << ID << endl;
  cout << "Age: " << age << endl;
  cout << "Salary: " << salary << endl;
}
