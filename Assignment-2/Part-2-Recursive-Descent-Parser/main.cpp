#include "RecursiveDescent.h"
#include "RecursiveDescent.cpp"
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

int main() {
    RecursiveDescent parser;
    ifstream Fin("sample.txt");

    if (!Fin) {
        cout << "Error: Could not open file sample.txt" << endl;
        return 1;
    }

    char lineBuffer[1024];
    int lineNum = 1;

    while (Fin.getline(lineBuffer, 1024)) {
        // Simple check to skip whitespace-only lines
        bool isEmpty = true;
        for (int i = 0; lineBuffer[i] != '\0'; i++) {
            if (!isspace(lineBuffer[i])) {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) continue;

        cout << "Line " << lineNum++ << ": ";
        parser.parseExpression(lineBuffer);
    }

    Fin.close();
    return 0;
}