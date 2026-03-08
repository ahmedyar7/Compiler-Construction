#include <fstream>
#include <iostream>

using namespace std;

const int MAX_STATES = 10;
const int MAX_CHARS = 256;

class DFA {
   private:
    int startState;
    bool acceptedState[MAX_STATES];
    int transitionMatrix[MAX_STATES][MAX_CHARS];

   public:
    DFA(int state) : startState(state) {
        for (int i = 0; i < MAX_STATES; i++) {
            for (int j = 0; j < MAX_CHARS; j++) {
                transitionMatrix[i][j] = -1;
            }
            acceptedState[i] = false;
        }
    }

    void addTransition(int currentState, char input, int nextState) {
        if (currentState < MAX_STATES && nextState < MAX_STATES) {
            transitionMatrix[currentState][input] = nextState;
        }
    }

    void addAcceptedState(int state) {
        if (state < MAX_STATES) {
            acceptedState[state] = true;
        }
    }

    bool process(const char* inputStr) {
        int currentState = startState;
        int i = 0;

        while (inputStr[i] != '\0') {
            currentState = transitionMatrix[currentState][inputStr[i]];
            if (currentState == -1) {
                return false;
            }
            i++;
        }
        return acceptedState[currentState];
    }
};
int main() {
    // +------------------------------------------------------+
    // | Setup the DFA (Accepts binary strings ending in "11")|
    // +------------------------------------------------------+
    DFA dfa(0);

    dfa.addAcceptedState(2);

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
        cerr << "Error: Could not open 'input.txt'. Please create it in the "
                "working directory.\n";
        return 1;
    }

    // Buffer to hold our inputs. Assumes no single string exceeds 255
    // characters.
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