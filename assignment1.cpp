#include <iostream>
#include <string>
#include <cctype>

using namespace std;

void identifyToken(const string& token);
bool isKeyword(const string& token);
bool isDataType(const string& token);
bool isSpecialChar(char c);

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
            if (isSpecialChar(c)) {
                cout << c << " is a special character.\n";
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
    } else {
        cout << token << " is an identifier.\n";
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

bool isSpecialChar(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' || c == ',');
}
