#include <iostream> // Include input/output stream library
#include <stdlib.h> // Include standard library for functions like exit()
#include<conio.h> // Include console input/output library (specific to some environments)
using namespace std; // Using the standard namespace

/*
Grammar:
E->TE'
E'->+TE'|-TE'|null
T-> FT'
T'->*FT'|/FT'|null
F-> id|num|(E)
*/

int count = 0; // Global variable to keep track of current position in the input string

// Function prototypes
void E();
void Ed();
void T();
void Td();
void F();

string expr; // Global variable to store the input expression

int main() {
  cin >> expr; // Take input expression from the user
  int l = expr.length(); // Get length of input expression
  expr += "$"; // Append '$' to the end of the expression to indicate end of input
  E(); // Call the starting symbol of the grammar
  if (l == count) { // If entire expression is parsed
    cout << "Accepted" << endl; // Print Accepted
    getchar(); // Wait for user input
  } else
    cout << "Rejected" << endl; // Print Rejected
  getchar(); // Wait for user input
}

void E() {
  cout << "E->TE'" << endl; // Print production rule being applied
  getchar(); // Wait for user input
  T(); // Call non-terminal T
  Ed(); // Call non-terminal E'
}

void Ed() {
  if (expr[count] == '+') { // If next symbol is '+'
    count++; // Move to the next symbol
    cout << "E'->+TE'" << endl; // Print production rule being applied
    getchar(); // Wait for user input
    T(); // Call non-terminal T
    Ed(); // Call non-terminal E'
  } else if (expr[count] == '-') { // If next symbol is '-'
    count++; // Move to the next symbol
    cout << "E'->-TE'" << endl; // Print production rule being applied
    getchar(); // Wait for user input
    T(); // Call non-terminal T
    Ed(); // Call non-terminal E'
  } else {
    cout << "E'->null" << endl; // If next symbol is not '+' or '-', print null production
    getchar(); // Wait for user input
  }
}

void T() {
  cout << "T->FT'" << endl; // Print production rule being applied
  getchar(); // Wait for user input
  F(); // Call non-terminal F
  Td(); // Call non-terminal T'
}

void Td() {
  if (expr[count] == '*') { // If next symbol is '*'
    count++; // Move to the next symbol
    cout << "T'->*FT'" << endl; // Print production rule being applied
    getchar(); // Wait for user input
    F(); // Call non-terminal F
    Td(); // Call non-terminal T'
  } else if (expr[count] == '/') { // If next symbol is '/'
    count++; // Move to the next symbol
    cout << "T'->/FT'" << endl; // Print production rule being applied
    getchar(); // Wait for user input
    F(); // Call non-terminal F
    Td(); // Call non-terminal T'
  } else {
    cout << "T'->null" << endl; // If next symbol is neither '*' nor '/', print null production
    getchar(); // Wait for user input
  }
}

void F() {
  if (isalpha(expr[count])) { // If next symbol is alphabetic
    count++; // Move to the next symbol
    cout << "F->id" << endl; // Print production rule being applied
    getchar(); // Wait for user input
  } else if (isdigit(expr[count])) { // If next symbol is a digit
    count++; // Move to the next symbol
    cout << "F->digit" << endl; // Print production rule being applied
    getchar(); // Wait for user input
  } else if (expr[count] == '(') { // If next symbol is '('
    count++; // Move to the next symbol
    cout << "F->(E)" << endl; // Print production rule being applied
    getchar(); // Wait for user input
    E(); // Call non-terminal E
    if (expr[count] != ')') { // If ')' does not follow '('
      cout << "Rejected" << endl; // Print Rejected
      getchar(); // Wait for user input
      exit(0); // Exit the program
    }
    count++; // Move to the next symbol
  } else {
    cout << "Rejected" << endl; // If none of the conditions match, print Rejected
    getchar(); // Wait for user input
    exit(0); // Exit the program
  }
}
