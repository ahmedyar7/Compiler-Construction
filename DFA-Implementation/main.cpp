#include <iostream>
#include <fstream>

using namespace std;


// Define maximums to replace dynamic data structures
const int MAX_STATES = 10;
const int MAX_CHARS = 256; // Standard extended ASCII size

class DFA {
private:
    // 2D array replaces map. 
    // transitions[state][character_ascii] = next_state
    int transitions[MAX_STATES][MAX_CHARS];
    
    // Boolean array replaces set for quick O(1) lookups
    bool acceptStates[MAX_STATES];
    
    int startState;

public:
    DFA(int start) : startState(start) {
        // Initialize all transitions to -1 (indicating no valid path / dead state)
        for (int i = 0; i < MAX_STATES; ++i) {
            for (int j = 0; j < MAX_CHARS; ++j) {
                transitions[i][j] = -1;
            }
            // Initialize all states as non-accepting
            acceptStates[i] = false;
        }
    }

    void addTransition(int currentState, char input, int nextState) {
        // Prevent out-of-bounds array access
        if (currentState < MAX_STATES && nextState < MAX_STATES) {
            // Cast char to unsigned char to handle extended ASCII safely
            transitions[currentState][(unsigned char)input] = nextState;
        }
    }

    void addAcceptState(int state) {
        if (state < MAX_STATES) {
            acceptStates[state] = true;
        }
    }

    // Accepts a C-style character array instead of string
    bool process(const char* inputStr) {
        int currentState = startState;
        int i = 0;

        // Iterate until we hit the null terminator '\0' of the char array
        while (inputStr[i] != '\0') {
            char c = inputStr[i];
            
            // Look up the next state in our 2D array
            currentState = transitions[currentState][(unsigned char)c];
            
            // If we hit a -1, there was no transition rule defined
            if (currentState == -1) {
                return false; 
            }
            i++;
        }

        // Return true if the final state is marked as true in our boolean array
        return acceptStates[currentState];
    }
};

int main() {
    // ---------------------------------------------------------
    // Setup the DFA (Accepts binary strings ending in "11")
    // ---------------------------------------------------------
    DFA dfa(0);

    dfa.addAcceptState(2);

    dfa.addTransition(0, '0', 0);
    dfa.addTransition(0, '1', 1);

    dfa.addTransition(1, '0', 0);
    dfa.addTransition(1, '1', 2);

    dfa.addTransition(2, '0', 0);
    dfa.addTransition(2, '1', 2);

    // ---------------------------------------------------------
    // File Handling & Processing
    // ---------------------------------------------------------
    ifstream inputFile("input.txt");

    // Check if the file opened successfully
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open 'input.txt'. Please create it in the working directory.\n";
        return 1;
    }

    // Buffer to hold our inputs. Assumes no single string exceeds 255 characters.
    char buffer[256]; 

    cout << "Reading from input.txt...\n";
    cout << "-------------------------\n";

    // Read word by word from the file into our char array
    while (inputFile >> buffer) {
        if (dfa.process(buffer)) {
            cout << "String \"" << buffer << "\" is ACCEPTED.\n";
        } else {
            cout << "String \"" << buffer << "\" is REJECTED.\n";
        }
    }

    // Close the file stream cleanly
    inputFile.close();

    return 0;
}