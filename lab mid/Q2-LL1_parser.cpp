#include <iostream>
#include <string>

using namespace std;

class Parser {
private:
    string input_string;
    size_t index;
    char current_token;

public:
    Parser(const string& input) : input_string(input), index(0), current_token(input[0]) {}

    bool match(char expected_token) {
        if (current_token == expected_token) {
            index++;
            if (index < input_string.length()) {
                current_token = input_string[index];
            }
            return true;
        }
        return false;
    }

    bool parse_S() {
        if (parse_X() && match('$')) {
            return true;
        }
        return false;
    }

    bool parse_X() {
        if (parse_Y() && parse_X_prime()) {
            return true;
        }
        return false;
    }

    bool parse_X_prime() {
        if (match('%')) {
            if (parse_Y() && parse_X_prime()) {
                return true;
            }
        }
        return true;
    }

    bool parse_Y() {
        if (parse_Z() && parse_Y_prime()) {
            return true;
        }
        return false;
    }

    bool parse_Y_prime() {
        if (match('&')) {
            if (parse_Z() && parse_Y_prime()) {
                return true;
            }
        }
        return true;
    }

    bool parse_Z() {
        if (match('k') && parse_X() && match('k')) {
            return true;
        }
        else if (match('g')) {
            return true;
        }
        return false;
    }

    bool parse() {
        return parse_S();
    }
};

int main() {
    string input;
    cout << "Enter a string to parse: ";
    cin >> input;

    Parser parser(input);
    if (parser.parse()) {
        cout << "String is in the language" << endl;
    }
    else {
        cout << "String is not in the language" << endl;
    }

    return 0;
}
