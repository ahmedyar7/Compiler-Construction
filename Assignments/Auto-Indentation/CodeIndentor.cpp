#include "CodeIndentor.h"

#include <iostream>

using namespace std;

CodeIndentor::CodeIndentor() { m_indentLevel = 0; }

void CodeIndentor::printIndentation(ofstream& outFile, int level) {
    for (int i = 0; i < level; i++) {
        cout << "    ";
        outFile << "    ";
    }
}

bool CodeIndentor::isEmpty(char* line) {
    int i = 0;
    while (line[i] != '\0') {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r' &&
            line[i] != '\t') {
            return false;
        }
        i++;
    }
    return true;  // Return true only if we reached end of string without
                  // finding chars
}

void CodeIndentor::processInputFile(const char* inputFileName,
                                    const char* outputFileName) {
    ifstream inFile(inputFileName);
    ofstream outFile(outputFileName);

    if (!inFile || !outFile) {
        cout << "--- UNABLE TO OPEN THE INPUT OR OUTPUT FILE ---" << endl;
        return;
    }

    char line[1024];

    while (inFile.getline(line, 1024)) {
        if (isEmpty(line)) continue;

        bool hasOpen = false;
        bool hasClose = false;

        int i = 0;
        while (line[i] != '\0') {
            if (line[i] == '{') hasOpen = true;
            if (line[i] == '}') hasClose = true;
            i++;
        }

        if (hasClose) {
            m_indentLevel--;
        }

        if (m_indentLevel < 0) m_indentLevel = 0;

        printIndentation(outFile, m_indentLevel);

        // Strip leading spaces
        int start = 0;
        while (line[start] == ' ' || line[start] == '\t') {
            start++;
        }

        cout << &line[start] << endl;
        outFile << &line[start] << endl;

        if (hasOpen) {
            m_indentLevel++;
        }
    }

    inFile.close();
    outFile.close();
    cout << "\n---- Indentation Complete ----\n";
}