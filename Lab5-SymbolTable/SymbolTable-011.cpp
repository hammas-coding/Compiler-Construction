// C++ implementation of Symbol Table using the concept of Hashing with separate chaining
#include <iostream> // Include input/output stream library
using namespace std; // Using the standard namespace

const int MAX = 100; // Maximum size of the symbol table

// Class representing a node in the symbol table
class Node {
    string identifier, scope, type; // Properties of the identifier
    int lineNo; // Line number where the identifier is found
    Node* next; // Pointer to the next node in case of collision

public:
    Node() {
        next = NULL; // Constructor to initialize next pointer to NULL
    }

    // Constructor to initialize properties of the node
    Node(string key, string value, string type, int lineNo) {
        this->identifier = key;
        this->scope = value;
        this->type = type;
        this->lineNo = lineNo;
        next = NULL;
    }

    // Function to print the details of the identifier
    void print() {
        cout << "Identifier's Name:" << identifier
            << "\nType:" << type
            << "\nScope: " << scope
            << "\nLine Number: " << lineNo << endl;
    }
    friend class SymbolTable; // Allowing SymbolTable class to access private members
};

// Class representing the symbol table
class SymbolTable {
    Node* head[MAX]; // Array of pointers to nodes (buckets) for separate chaining

public:
    SymbolTable() {
        for (int i = 0; i < MAX; i++)
            head[i] = NULL; // Initialize all buckets to NULL
    }

    int hashf(string id); // Declaration of hash function
    bool insert(string id, string scope, string Type, int lineno); // Function to insert an identifier
    string find(string id); // Function to find an identifier
    bool deleteRecord(string id); // Function to delete an identifier
    bool modify(string id, string scope, string Type, int lineno); // Function to modify an identifier
};

// Function to modify an identifier
bool SymbolTable::modify(string id, string s, string t, int l) {
    int index = hashf(id); // Compute the hash value of the identifier
    Node* start = head[index]; // Get the starting node of the bucket

    if (start == NULL)
        return false; // Return false if the bucket is empty

    while (start != NULL) { // Iterate through the linked list in the bucket
        if (start->identifier == id) { // If the identifier is found
            start->scope = s; // Update its scope
            start->type = t; // Update its type
            start->lineNo = l; // Update its line number
            return true; // Return true indicating successful modification
        }
        start = start->next; // Move to the next node in the linked list
    }

    return false; // Return false if the identifier is not found
}

// Function to delete an identifier
bool SymbolTable::deleteRecord(string id) {
    int index = hashf(id); // Compute the hash value of the identifier
    Node* tmp = head[index]; // Get the starting node of the bucket
    Node* par = head[index]; // Pointer to keep track of the previous node

    // Case when no identifier is present at that index
    if (tmp == NULL) {
        return false;
    }

    // Case when only one identifier is present
    if (tmp->identifier == id && tmp->next == NULL) {
        tmp->next = NULL;
        delete tmp; // Delete the node
        return true;
    }

    // Case when identifier is present but not at the head of the linked list
    while (tmp->identifier != id && tmp->next != NULL) {
        par = tmp; // Update previous node
        tmp = tmp->next; // Move to the next node
    }
    if (tmp->identifier == id && tmp->next != NULL) {
        par->next = tmp->next;
        tmp->next = NULL;
        delete tmp; // Delete the node
        return true;
    }

    // Case when identifier is present at the end of the linked list
    else {
        par->next = NULL;
        tmp->next = NULL;
        delete tmp; // Delete the node
        return true;
    }
    return false;
}

// Function to find an identifier
string SymbolTable::find(string id) {
    int index = hashf(id); // Compute the hash value of the identifier
    Node* start = head[index]; // Get the starting node of the bucket

    if (start == NULL)
        return "-1"; // Return -1 if the bucket is empty

    while (start != NULL) { // Iterate through the linked list in the bucket

        if (start->identifier == id) { // If the identifier is found
            start->print(); // Print its details
            return start->scope; // Return its scope
        }

        start = start->next; // Move to the next node in the linked list
    }

    return "-1"; // Return -1 if the identifier is not found
}

// Function to insert an identifier
bool SymbolTable::insert(string id, string scope, string Type, int lineno) {
    int index = hashf(id); // Compute the hash value of the identifier
    Node* p = new Node(id, scope, Type, lineno); // Create a new node with the given properties

    if (head[index] == NULL) { // If the bucket is empty
        head[index] = p; // Set the new node as the head of the bucket
        cout << "\n" << id << " inserted"; // Print a message indicating successful insertion
        return true;
    }

    else { // If the bucket is not empty
        Node* start = head[index]; // Get the starting node of the bucket
        while (start->next != NULL)
            start = start->next; // Traverse the linked list to the end

        start->next = p; // Insert the new node at the end of the linked list
        cout << "\n" << id << " inserted"; // Print a message indicating successful insertion
        return true;
    }

    return false;
}

// Function to compute the hash value of an identifier
int SymbolTable::hashf(string id) {
    int asciiSum = 0; // Variable to store the sum of ASCII values of characters in the identifier

    for (int i = 0; i < id.length(); i++) { // Iterate through each character of the identifier
        asciiSum = asciiSum + id[i]; // Add the ASCII value of the character to the sum
    }

    return (asciiSum % 100); // Return the remainder when the sum is divided by 100 (hash value)
}

// Driver code
int main() {
    SymbolTable st; // Create an instance of SymbolTable class
    string check; // Variable to store the result of find operation
    cout << "**** SYMBOL_TABLE ****\n"; // Print a header indicating the symbol table

    // Insert 'if' into the symbol table
    if (st.insert("if", "local", "keyword", 4))
        cout << " -successfully"; // Print a message indicating successful insertion
    else
        cout << "\nFailed to insert.\n"; // Print a message indicating failed insertion

    // Insert 'number' into the symbol table
    if (st.insert("number", "global", "variable", 2))
        cout << " -successfully\n\n"; // Print a message indicating successful insertion
    else
        cout << "\nFailed to insert\n"; // Print a message indicating failed insertion

    // Find 'if' in the symbol table
    check = st.find("if");
    if (check != "-1")
        cout << "Identifier Is present\n"; // Print a message indicating 'if' is present
    else
        cout << "\nIdentifier Not Present\n"; // Print a message indicating 'if' is not present

    // Delete 'if' from the symbol table
    if (st.deleteRecord("if"))
        cout << "if Identifier is deleted\n"; // Print a message indicating successful deletion
    else
        cout << "\nFailed to delete\n"; // Print a message indicating failed deletion

    // Modify 'number' in the symbol table
    if (st.modify("number", "global", "variable", 3))
        cout << "\nNumber Identifier updated\n"; // Print a message indicating successful modification

    // Find and print 'number' from the symbol table
    check = st.find("number");
    if (check != "-1")
        cout << "Identifier Is present\n"; // Print a message indicating 'number' is present
    else
        cout << "\nIdentifier Not Present"; // Print a message indicating 'number' is not present

    return 0; // Exit the program
}
