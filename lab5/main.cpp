#include <sstream>
#include <string>

#include "BST.h"
#include "Employee.h"
using namespace std;

// Global variables
BST* nameTree = new BST("name");  // search name and autocomplete name
BST* IDTree = new BST("ID");
BST* ageTree = new BST("age");

void addEmployee(stringstream& ss);
void searchEmployee(stringstream& ss);
void autocompleteEmployee(stringstream& ss);
void searchAgeRangeEmployee(stringstream& ss);

bool foundMoreArgs(stringstream& lineStream);
bool getDouble(stringstream& lineStream, double& dvalue);
bool getInt(stringstream& lineStream, int& iValue);
bool getString(stringstream& ss, string& s);

int main() {
  string line;
  string command;
  cout << "Welcome to our employee database!" << endl;
  cout << "The database is empty now!" << endl;

  cout << ">>> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "employee") {
      addEmployee(lineStream);
    } else if (command == "search") {
      searchEmployee(lineStream);
    } else if (command == "autocomplete") {
      autocompleteEmployee(lineStream);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << ">>> ";  // Prompt for input
    getline(cin, line);
  }

  // You have to make sure all dynamically allocated memory is freed
  // before return 0
  delete nameTree;
  delete IDTree;
  delete ageTree;

  return 0;
}

void addEmployee(stringstream& lineStream) {
  // employee <ID> <firstName> <lastName> <age> <salary>
  string firstName, lastName;
  int ID;
  double age, salary;
  if (!getInt(lineStream, ID) || !getString(lineStream, firstName) ||
      !getString(lineStream, lastName) || !getDouble(lineStream, age) ||
      !getDouble(lineStream, salary)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Add the employee to the available trees and print "Error: ID already
  // exists" if ID already exists
  if(IDTree->searchID(ID) != NULL) {
    cout << "Error: ID already exists" << endl;
    return;
  }

  Employee* newEmployeeName = new Employee(firstName, lastName, ID, age, salary);
  Employee* newEmployeeID = new Employee(firstName, lastName, ID, age, salary);
  Employee* newEmployeeSalary = new Employee(firstName, lastName, ID, age, salary);

  nameTree->insert(newEmployeeName);
  IDTree->insert(newEmployeeID);
  ageTree->insert(newEmployeeSalary);
}

void searchAgeRangeEmployee(stringstream& lineStream) {
  // search age <lowAge> <highAge>
  // can assume lowAge is always less than or equal highAge
  double lowAge, highAge;
  if (!getDouble(lineStream, lowAge) || !getDouble(lineStream, highAge)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Search for employees with age between lowAge and highAge inclusive
  ageTree->searchAgeRange(lowAge, highAge);
}

void searchIDEmployee(stringstream& lineStream) {
  // search ID <ID>
  int ID;
  if (!getInt(lineStream, ID)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }

  // Search for employee with ID and print their info.
  Employee* employee = IDTree->searchID(ID);

  if(employee == NULL) {
    cout << ID << " ID does not exist" << endl;
  } 
  else {
    employee->print();
  }
}

void searchEmployee(stringstream& lineStream) {
  string mode;
  if (!getString(lineStream, mode)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  
  if (mode == "ID") {
    searchIDEmployee(lineStream);
  } 
  else if (mode == "age") {
    searchAgeRangeEmployee(lineStream);
  }
}

void autocompleteEmployee(stringstream& ss) {
  // autocomplete <prefix of a name>
  // read whatever is entered by the user
  // and print all employees that have names that start the same way
  // you should ignore spaces in names
  string prefix;
  getline(ss, prefix);

  // Search for employees with names having the same prefix
  nameTree->autocomplete(prefix);
}

bool getString(stringstream& lineStream, string& s) {
  lineStream >> s;
  if (lineStream.fail()) {
    return false;
  }
  return true;
}

bool getInt(stringstream& lineStream, int& iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream& lineStream, double& dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream& lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}
