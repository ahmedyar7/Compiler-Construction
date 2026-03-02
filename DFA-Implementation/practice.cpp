#include <fstream>
#include <iostream>

using namespace std;

const int MAX_STATES = 10;
const int MAX_CHARS = 256;

class DFA {
   private:
    int startState;
    int transition[MAX_STATES][MAX_CHARS];
    bool acceptedStates[MAX_STATES];

   public:
    DFA(int start) : startState(start) {
        for (int i = 0; i < MAX_STATES; i++) {
            for (int j = 0; j < MAX_CHARS; j++) {
                transition[i][j] = -1;
            }
            acceptedStates[i] = false;
        }
    }

    void addTransition(int currentState, char input, int nextState) {
        if (currentState < MAX_STATES && nextState < MAX_STATES) {
            transition[currentState][input] = nextState;
        }
    }

    void addAcceptedState(int state) {
        if (state < MAX_STATES) {
            acceptedStates[state] = true;
        }
    }

    bool process(const char* inputStr) {
        int currentState = startState;
        int i = 0;

        while (inputStr[i] != '\0') {
            char c = inputStr[i];

            currentState = transition[currentState][c];
            if (currentState == -1) {
                return false;
            }

            i++;
        }
        return acceptedStates[currentState];
    }
};

int main() {

    DFA dfa(0);

    dfa.addAcceptedState(2);

    dfa.addTransition(0,'0',0);
    dfa.addTransition(0,'1',1);

    dfa.addTransition(1,'0',0);
    dfa.addTransition(1,'1',2);

    dfa.addTransition(2,'0',0);
    dfa.addTransition(2,'1',2);

    // Taking input from the file
    

}