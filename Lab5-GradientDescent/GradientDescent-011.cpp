#include <iostream>
#include <stdlib.h>
#include<conio.h>
using namespace std;
/*
E->TE'
E'->+TE'|-TE'|null
T-> FT'
T'->*FT'|/FT'|null
F-> id|num|(E)
*/
int count = 0;
void E();
void Ed();
void T();
void Td();
void F();

string expr;

int main() {
  cin >> expr;
  int l = expr.length();
  expr += "$";
  E();
  if (l == count)
  {
  
    cout << "Accepted" << endl;
    getchar();
  }else
  
    cout << "Rejected" << endl;
getchar();
}

void E() {
  cout << "E->TE'" << endl;
  getchar();
  T();
  Ed();
}

void Ed() {
  if (expr[count] == '+') {
    count++;
    cout << "E'->+TE'" << endl;
   getchar();
    T();
    Ed();
  }

  else if (expr[count] == '-') {
    count++;
    cout << "E'->-TE'" << endl;
    getchar();
	T();
    Ed();
  }

  else {
    cout << "E'->null" << endl;
  getchar();
  }
}

void T() {
  cout << "T->FT'" << endl;
  getchar();
  F();
  Td();
}

void Td() {
  if (expr[count] == '*') {
    count++;
    cout << "T'->*FT'" << endl;
    getchar();
	F();
    Td();
  }

  else if (expr[count] == '/') {
    count++;
    cout << "T'->/FT'" << endl;
    getchar();
	F();
    Td();
  }

  else {
    cout << "T'->null" << endl;
  getchar();
  }
}

void F() {
  if (isalpha(expr[count])) {
    count++;
    cout << "F->id" << endl;
  getchar();
  } else if (isdigit(expr[count])) {
    count++;
    cout << "F->digit" << endl;
  getchar();
  } else if (expr[count] == '(') {
    count++;
    cout << "F->(E)" << endl;
    getchar();
	E();
    if (expr[count] != ')') {
      cout << "Rejected" << endl;
      getchar();
	  exit(0);
    }
    count++;
  } else {
    cout << "Rejected" << endl;
    getchar();
	exit(0);
  }
}
