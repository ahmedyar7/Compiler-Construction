#include <fstream>
#include <iostream>

using namespace std;

class CodeIndentor {
   private:
    int indentLevel;

    // helper function to print the indentation
    void printIndentation(ofstream& outFile, int level) {
        for (int i = 0; i < level; i++) {
            cout << "    ";
            outFile << "    ";
        }
    }

    bool isEmpty(char* line) {
        int i = 0;
        while (line[i] != '\0') {
            if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r' &&
                line[i] != '\t') {
                return false;
            }
            i++;
            return true;
        }
    }

   public:
    CodeIndentor() { indentLevel = 0; }

    void processInputFile(const char* inputFileName,
                          const char* outputFileName) {
        ifstream inFile(inputFileName);
        ofstream outFile(outputFileName);

        if (!inFile || !outFile) {
            cout << "--- UNABLE TO OPEN THE INPUT OR OUTPUT FILE ---" << endl;
            return;
        }

        char line[1024];

        while (inFile.getline(line, 1024)) {
            if (isEmpty(line)) {
                continue;
            }

            bool hasOpen = false;
            bool hasClose = false;

            int i = 0;
            while (line[i] != '\0') {
                if (line[i] == '{') hasOpen = true;
                if (line[i] == '}') hasClose = true;

                i++;
            }

            if (hasClose) {
                indentLevel--;
            }

            if (indentLevel < 0) {
                indentLevel = 0;
            }
            printIndentation(outFile, indentLevel);

            // Print Indentation
            int start = 0;
            while (line[start] == ' ' || line[start] == '\t') {
                start++;
            }
            cout << &line[start] << endl;
            outFile << &line[start] << endl;

            if (hasOpen) {
                indentLevel++;
            }
        }
        inFile.close();
        outFile.close();
        cout << "\n---- Indentation Complete ----\n";
    }
};

int main() {
    CodeIndentor().processInputFile("./input.cpp", "./output.cpp");

    return 0;
}