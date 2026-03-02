#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

int dfa = 0;

// 3 Function = 3 State Machine
void q0(char c) {
    if (c == '0') {
        dfa = 1;  // q0 -> [0] -> q1
    } else if (c == '1') {
        dfa = 2;  // q0 -> [1] -> q2
    } else {
        dfa = -1;  // INVALID STATE TRANSITION...
    }
}

void q1(char c) {
    if (c == '0') {
        dfa = 0;  // q1 -> [0] -> q0
    } else if (c == '1') {
        dfa = 2;  // q1 -> [1] -> q2
    } else {
        dfa = -1;  // Invalid state transition...
    }
}

void q2(char c) {
    if (c == '0') {
        dfa = 2;  // q1 -> [0] -> q2
    } else if (c == '1') {
        dfa = 2;  // q1 -> [1] -> q2
    } else {
        dfa = -1;  // Invalid state transition...
    }
}

int isAccepted(char* str) {
    int i, len = strlen(str);

    for (int i = 0; i < len; i++) {
        if (dfa == 0) {
            q0(str[i]);
        }
        else if (dfa == 1) {
            q1(str[i]);
        }
        else if (dfa == 2) {
            q2(str[i]);
        } 
        
        else {
            return 0;
        }
    }

    if (dfa == 2) {
        return 1;  // accepted
    } else {
        return 0;  // rejected
    }
}

int main() {
    char str[] = "001";

    if (isAccepted(str)) {
        cout << "Accepted\n";
    }
    else{
        cout << "Rejected\n";
    }

    return 0;
}