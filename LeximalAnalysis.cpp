#include <iostream>
#include <string>
#include <cctype>

using namespace std;

void identifyToken(const string& token);
bool isKeyword(const string& token);
bool isDataType(const string& token);
bool isOperator(char c);
bool isSpecialChar(char c);
bool isNumber(const string& token);
bool isIdentifier(const string& token);

int main() {
    string inputCode;
    string token;

    cout << "Enter the C++ code:\n";
    getline(cin, inputCode);

    for (char c : inputCode) {
        if (isalnum(c) || c == '_') {
            token += c;
        } else {
            if (!token.empty()) {
                identifyToken(token);
                token.clear(); // Reset token
            }
            if (isOperator(c)) {
                string operatorToken(1, c);
                identifyToken(operatorToken);
            } else if (isSpecialChar(c)) {
                string specialCharToken(1, c);
                identifyToken(specialCharToken);
            }
        }
    }

    if (!token.empty()) {
        identifyToken(token);
    }

    cout << "Lexical analysis completed.\n";

    return 0;
}

void identifyToken(const string& token) {
    if (isKeyword(token)) {
        cout << token << " is a keyword.\n";
    } else if (isDataType(token)) {
        cout << token << " is a data type.\n";
    } else if (isNumber(token)) {
        cout << token << " is a number.\n";
    } else if (isIdentifier(token)) {
        cout << token << " is an identifier.\n";
    } else if (token.size() == 1) {
        if (isOperator(token[0])) {
            cout << token << " is an operator.\n";
        } else if (isSpecialChar(token[0])) {
            cout << token << " is a special character.\n";
        }
    } else {
        cout << token << " is an unrecognized token.\n";
    }
}

bool isKeyword(const string& token) {
    static const string keywords[] = {"for", "while", "do", "int", "float", "char", "double", "static", "switch", "case"};
    for (const string& keyword : keywords) {
        if (token == keyword) {
            return true;
        }
    }
    return false;
}

bool isDataType(const string& token) {
    static const string dataTypes[] = {"int", "float", "char"};
    for (const string& dataType : dataTypes) {
        if (token == dataType) {
            return true;
        }
    }
    return false;
}

bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}

bool isSpecialChar(char c) {
    return (c == '<' || c == '>' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']');
}

bool isNumber(const string& token) {
    for (char c : token) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isIdentifier(const string& token) {
    if (isdigit(token[0])) {
        return false;
    }
    for (char c : token) {
        if (!isalnum(c) && c != '_') {
            return false;
        }
    }
    return true;
}
