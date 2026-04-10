#include <iostream>
using namespace std;

const int MAX_NFA_STATES = 8;
const int MAX_DFA_STATES = 20;

const int SYMBOL_A = 0;
const int SYMBOL_B = 1;
const int SYMBOL_EPSILON = 2;

unsigned char nfa[MAX_NFA_STATES][3] = {0};

bool stateInSet(unsigned char stateSet, int state) {
    return stateSet & (1 << state);
}

unsigned char addToSet(unsigned char stateSet, int state) {
    return stateSet | (1 << state);
}
unsigned char epsilonClosure(unsigned char startSet) {
    unsigned char closure = startSet;
    bool addedNewState = true;

    while (addedNewState) {
        addedNewState = false;
        for (int state = 0; state < MAX_NFA_STATES; state++) {
            if (stateInSet(closure, state)) {
                unsigned char epsilonTargets = nfa[state][SYMBOL_EPSILON];
                if ((closure | epsilonTargets) != closure) {
                    closure |= epsilonTargets;
                    addedNewState = true;
                }
            }
        }
    }
    return closure;
}

unsigned char move(unsigned char stateSet, int symbol) {
    unsigned char reachable = 0;
    for (int state = 0; state < MAX_NFA_STATES; state++) {
        if (stateInSet(stateSet, state)) {
            reachable |= nfa[state][symbol];
        }
    }
    return reachable;
}

int main() {
    nfa[0][SYMBOL_EPSILON] = addToSet(addToSet(addToSet(0, 1), 2), 3);
    nfa[0][SYMBOL_B] = addToSet(0, 5);

    nfa[1][SYMBOL_A] = addToSet(0, 3);
    nfa[1][SYMBOL_B] = addToSet(0, 7);

    nfa[2][SYMBOL_A] = addToSet(0, 5);

    nfa[3][SYMBOL_B] = addToSet(0, 7);

    nfa[5][SYMBOL_B] = addToSet(0, 7);

    nfa[7][SYMBOL_A] = addToSet(0, 6);

    unsigned char dfaStateSets[MAX_DFA_STATES];

    int dfaTransitions[MAX_DFA_STATES][2];
    int dfaStateCount = 0;

    for (int i = 0; i < MAX_DFA_STATES; i++)
        dfaTransitions[i][0] = dfaTransitions[i][1] = -1;

    unsigned char startSet = epsilonClosure(addToSet(0, 0));
    dfaStateSets[dfaStateCount++] = startSet;

    for (int currentDfaState = 0; currentDfaState < dfaStateCount;
         currentDfaState++) {
        for (int symbol = 0; symbol <= 1; symbol++) {
            unsigned char movedSet =
                move(dfaStateSets[currentDfaState], symbol);
            if (movedSet == 0) continue;

            unsigned char newDfaSet = epsilonClosure(movedSet);

            int existingDfaState = -1;
            for (int j = 0; j < dfaStateCount; j++) {
                if (dfaStateSets[j] == newDfaSet) {
                    existingDfaState = j;
                    break;
                }
            }

            if (existingDfaState == -1) {
                dfaTransitions[currentDfaState][symbol] = dfaStateCount;
                dfaStateSets[dfaStateCount++] = newDfaSet;
            } else {
                dfaTransitions[currentDfaState][symbol] = existingDfaState;
            }
        }
    }

    cout << "\nSTATE | ON 'a' | ON 'b' | ACCEPT\n";
    cout << "-------------------------------\n";

    for (int i = 0; i < dfaStateCount; i++) {
        // Simple Label (A, B, C...)
        cout << (char)('A' + i) << "     |   ";

        // Transition on 'a'
        if (dfaTransitions[i][SYMBOL_A] != -1)
            cout << (char)('A' + dfaTransitions[i][SYMBOL_A]);
        else
            cout << "-";

        cout << "    |   ";

        // Transition on 'b'
        if (dfaTransitions[i][SYMBOL_B] != -1)
            cout << (char)('A' + dfaTransitions[i][SYMBOL_B]);
        else
            cout << "-";

        cout << "    |  ";

        // Simple Yes/No for acceptance
        cout << (stateInSet(dfaStateSets[i], 6) ? "Yes" : "No") << "\n";
    }

    return 0;
    ;
}