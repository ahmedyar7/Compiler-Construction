#include <fstream>
#include <iostream>

using namespace std;

class CodeIndenter {
   private:
    int indentLevel;

    // Helper to print correct number of tabs
    void printIndentation(ofstream& outFile, int level) {
        for (int i = 0; i < level; i++) {
            cout << "    ";     // Display 4 spaces as a tab
            outFile << "    ";  // Write to file
        }
    }

    // Manual function to check if a line is just whitespace
    bool isEmpty(char* line) {
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' &&
                line[i] != '\r')
                return false;
        }
        return true;
    }

   public:
    CodeIndenter() { indentLevel = 0; }

    void processCode(const char* inputFileName, const char* outputFileName) {
        ifstream inFile(inputFileName);
        ofstream outFile(outputFileName);

        if (!inFile || !outFile) {
            cout << "Error opening files!" << endl;
            return;
        }

        char line[500];  // Buffer for each line
        cout << "--- Indented Output ---\n" << endl;

        while (inFile.getline(line, 500)) {
            if (isEmpty(line)) continue;

            // Check for closing brace first to reduce indent before printing
            bool hasOpen = false;
            bool hasClose = false;

            for (int i = 0; line[i] != '\0'; i++) {
                if (line[i] == '{') hasOpen = true;
                if (line[i] == '}') hasClose = true;
            }

            if (hasClose) indentLevel--;

            // Print indentation
            if (indentLevel < 0) indentLevel = 0;
            printIndentation(outFile, indentLevel);

            // Print the actual line content (skipping leading spaces)
            int start = 0;
            while (line[start] == ' ' || line[start] == '\t') start++;

            cout << &line[start] << endl;
            outFile << &line[start] << endl;

            if (hasOpen) indentLevel++;
        }

        inFile.close();
        outFile.close();
        cout << "\n--- Indentation Complete. File saved as " << outputFileName
             << " ---" << endl;
    }
};

int main() {
    CodeIndenter myTool;

    // Ensure "input.cpp" exists in your directory before running
    myTool.processCode("input.cpp", "output.cpp");

    return 0;
}