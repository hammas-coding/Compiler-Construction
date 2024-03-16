#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

enum Floor { GROUND, FLOOR_1, FLOOR_2, LIBRARY };

class Elevator {
private:
    Floor currentFloor;
    bool doorOpen;

public:
    Elevator() : currentFloor(GROUND), doorOpen(false) {}

    void moveToFloor(Floor floor) {
        if (floor != currentFloor) {
            closeDoor();
            while (currentFloor != floor) {
                if (currentFloor < floor) {
                    cout << "Moving up to ";
                    currentFloor = static_cast<Floor>(currentFloor + 1);
                } else {
                    cout << "Moving down to ";
                    currentFloor = static_cast<Floor>(currentFloor - 1);
                }
                printFloor(currentFloor);
                this_thread::sleep_for(chrono::seconds(1)); // Simulate movement time
            }
        }
        openDoor();
    }

    void openDoor() {
        cout << "✅ Door opened at ";
        printFloor(currentFloor);
        doorOpen = true;
        this_thread::sleep_for(chrono::seconds(3)); // Door open time
    }

    void closeDoor() {
        if (doorOpen) {
            cout << "❌ Door closed at ";
            printFloor(currentFloor);
            doorOpen = false;
            this_thread::sleep_for(chrono::seconds(1)); // Door close time
        }
    }

    void printFloor(Floor floor) {
        switch (floor) {
            case GROUND:
                cout << "Ground Floor" << endl;
                break;
            case FLOOR_1:
                cout << "Floor 1" << endl;
                break;
            case FLOOR_2:
                cout << "Floor 2" << endl;
                break;
            case LIBRARY:
                cout << "Library" << endl;
                break;
        }
    }
};

int main() {
    Elevator elevator;
    string userInput;

    cout << "Welcome to the Elevator System!" << endl;

    while (true) {
        cout << "Enter your command (G, 1, 2, L, ⬆, ⬇, ❌, ✅, or 'exit' to quit): ";
        cin >> userInput;

        if (userInput == "exit") {
            break;
        } else if (userInput == "G") {
            elevator.moveToFloor(GROUND);
        } else if (userInput == "1") {
            elevator.moveToFloor(FLOOR_1);
        } else if (userInput == "2") {
            elevator.moveToFloor(FLOOR_2);
        } else if (userInput == "L") {
            elevator.moveToFloor(LIBRARY);
        } else if (userInput == "⬆") {
            // Move up
            cout << "⬆ Moving up" << endl;
        } else if (userInput == "⬇") {
            // Move down
            cout << "⬇ Moving down" << endl;
        } else if (userInput == "❌") {
            elevator.closeDoor();
        } else if (userInput == "✅") {
            elevator.openDoor();
        } else {
            cout << "Invalid command. Please try again." << endl;
        }

        // Move elevator back to ground floor after each command
        elevator.moveToFloor(GROUND);
    }

    return 0;
}
