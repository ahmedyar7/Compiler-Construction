#include <cstring>
#include <iostream>

using namespace std;

const int MAX_STATES = 10;
const int MAX_CHARS = 256;

class DFA {
   private:
    int startState;
    int transitions_matrix[MAX_STATES][MAX_CHARS];
    bool acceptedStates[MAX_STATES];

   public:
    DFA(int start) : startState(start) {
        for (int i = 0; i < MAX_STATES; i++) {
            for (int j = 0; j < MAX_CHARS; j++) {
                transitions_matrix[i][j] = -1;
            }
            acceptedStates[i] = -1;
        }
    }

    void transition(int currentState, char input, int nextState) {
        while (currentState < MAX_STATES && nextState < MAX_STATES) {
            transitions_matrix[currentState][input] = nextState;
        }
    }

    void acceptState(int state) {
        if (state < MAX_STATES) {
            acceptedStates[state] = true;
        }
    }

    void process(const char* inputStr) {
        int currentState = startState;
        int i = 0;

        while (inputStr[i] != '\0') {
            currentState = transitions_matrix[currentState][inputStr[i]];

            if (currentState == -1) {
                acceptedStates[currentState] = false;
            }
            i++;
        }
    }
};

int main() { return 0; }