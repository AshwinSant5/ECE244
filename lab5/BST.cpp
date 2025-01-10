//
//  BST.cpp
//  Lab 5 Search Through A Database
//
//  Created by Author Name, Date
#include "BST.h"

#include "Employee.h"

BST::BST(string order_) {
  root = NULL;
  order = order_;
}

BST::~BST() {
  delete root;
}

//insert employee into the tree
void BST::insert(Employee* newEmployee) {
  if(root == NULL) {
    root = newEmployee;
    return;
  }

  Employee* current = root;

  while(true) {
    if(order == "ID") { 
      if(newEmployee->getID() <= current->getID()) {
        if(current->getLeft() == NULL) {
          current->setLeft(newEmployee);
          return;
        }
        else {
        current = current->getLeft();
        }
      } 
      else {
        if(current->getRight() == NULL) {
          current->setRight(newEmployee);
          return;
        }
        else {
        current = current->getRight();
        }
      }
    }

    else if(order == "name") {
      string curr = current->getName();
      string newEmp = newEmployee->getName();
      if(newEmp <= curr) {
        if(current->getLeft() == NULL) {
          current->setLeft(newEmployee);
          return;
        }
        else {
        current = current->getLeft();
        }
      } 
      else {
        if(current->getRight() == NULL) {
          current->setRight(newEmployee);
          return;
        }
        else {
        current = current->getRight();
        }
      }
    }
    
    else if (order == "age") {
      if(newEmployee->getAge() <= current->getAge()) {
        if(current->getLeft() == NULL) {
          current->setLeft(newEmployee);
          return;
        }
        else {
        current = current->getLeft();
        }
      } 
      else {
        if(current->getRight() == NULL) {
          current->setRight(newEmployee);
          return;
        }
        else {
        current = current->getRight();
        }
      }
    }
  }
}
 

// helper function for printInOrder
void printInOrderHelper(Employee* current) {
  if(current == NULL) {
    return;
  }
  printInOrderHelper(current->getLeft());
  current->print();
  printInOrderHelper(current->getRight());
}

// print the Employees in the tree in order
void BST::printInOrder() {
  printInOrderHelper(root);
}



// search for an employee ID
Employee* BST:: searchID(int ID) {

  Employee* current = root;

  while(current != NULL) {
    if(current->getID() == ID) {
      return current;
    } 
    else if(current->getID() < ID) {
      current = current->getRight();
    } 
    else {
      current = current->getLeft();
    }
  }

  return NULL;
}

// helper function for searchAgeRange
void searchAgeRangeHelper(Employee* current, double lowAge, double highAge) {
    if (current == NULL) {
    return;
  }
  // Search the left subtree
  if (current->getAge() >= lowAge) {
    searchAgeRangeHelper(current->getLeft(), lowAge, highAge);
  }
  // Print the current employee
  if (current->getAge() >= lowAge && current->getAge() <= highAge) {
    current->print();
  }
  // Search the right subtree
  if (current->getAge() <= highAge) {
    searchAgeRangeHelper(current->getRight(), lowAge, highAge);
  }
}

// search for employees within the age range
void BST::searchAgeRange(double lowAge, double highAge) {
  searchAgeRangeHelper(root, lowAge, highAge);
}


//helper function for autocomplete
void autocompleteHelper(Employee* current, string prefix) {
  if(current == NULL) {
    return;
  }

  string name = current->getName();
  //remove spaces from the prefix
  string prefixNoSpace = "";
  for(int i = 0; i < prefix.size(); i++) {
    if(prefix[i] != ' ') {
      prefixNoSpace += prefix[i];
    }
  }

  int length = name.length();
  int prefixLength = prefixNoSpace.length();

  // Check if the current name starts with the normalized prefix
  if (length >= prefixLength && name.compare(0, prefixLength, prefixNoSpace) == 0) {
    autocompleteHelper(current->getLeft(), prefix); // Check left subtree
    current->print();                               // Print matching employee
    autocompleteHelper(current->getRight(), prefix); // Check right subtree
  } else if (name < prefixNoSpace) {
    autocompleteHelper(current->getRight(), prefix);
  } else {
    autocompleteHelper(current->getLeft(), prefix);
  }
}

// Search for employees with names having the same prefix in the firstName + secondName
void BST::autocomplete(string prefix) {
  autocompleteHelper(root, prefix);
}

