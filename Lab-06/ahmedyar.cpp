#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

// ---------------------------------------------------------
// HELPER FUNCTIONS FOR CHARACTER CLASSIFICATION
// We use these custom functions instead of <cctype> or <cstring>
// to keep the code as raw and simple as possible.
// ---------------------------------------------------------

// Checks if a character is a digit (0-9)
bool isDigit(char c) { return c >= '0' && c <= '9'; }

// Checks if a character is an alphabet letter or an underscore
bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// Checks if a character is alphanumeric (letter, digit, or underscore)
bool isAlnum(char c) { return isAlpha(c) || isDigit(c); }

// Checks if a character is whitespace (spaces, tabs, newlines)
bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Custom string comparison to avoid using the <cstring> library
// Returns true if both char arrays are exactly identical
bool strEquals(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    // Make sure both strings end at the same length
    return str1[i] == '\0' && str2[i] == '\0';
}

// ---------------------------------------------------------
// MAIN LEXICAL ANALYZER ROUTINE
// ---------------------------------------------------------
int main() {
    // Open the input file for reading using basic C-style file I/O
    FILE* myfile = fopen("input.cpp", "r");

    // Check if the file opened successfully [cite: 78, 80]
    if (!myfile) {
        cout << "Error: Cannot open input.cpp!" << endl;
        return -1;
    }

    cout << "--- Starting Tokenization ---" << endl;

    // Read the very first character from the file
    char c = fgetc(myfile);

    // Loop until we hit the End Of File (EOF) marker
    while (c != EOF) {
        // 1. SKIP WHITESPACE
        // If the character is a space, tab, or newline, just read the next one
        // and continue
        if (isWhitespace(c)) {
            c = fgetc(myfile);
            continue;
        }

        // 2. HANDLE COMMENTS (starting with //) [cite: 34]
        if (c == '/') {
            char next_c = fgetc(myfile);
            // Check if it's a double slash
            if (next_c == '/') {
                cout << "COMMENT: //";
                c = fgetc(myfile);  // Move past the second slash

                // Keep reading and printing until the end of the line
                while (c != EOF && c != '\n') {
                    cout << c;
                    c = fgetc(myfile);
                }
                cout << endl;
                continue;  // The current 'c' is now '\n', which will be caught
                           // by whitespace skipper
            } else {
                // If it was just a single slash, we mark it unknown and process
                // the next char
                cout << "UNKNOWN: /" << endl;
                c = next_c;
                continue;
            }
        }

        // 3. HANDLE OPERATORS (=, *, &) [cite: 33]
        if (c == '=' || c == '*' || c == '&') {
            cout << "OPERATOR: " << c << endl;
            c = fgetc(myfile);  // Move to next character
            continue;
        }

        // 4. HANDLE PUNCTUATION ( ( ) { } , ; ) [cite: 33]
        if (c == '(' || c == ')' || c == '{' || c == '}' || c == ',' ||
            c == ';') {
            cout << "PUNCTUATION: " << c << endl;
            c = fgetc(myfile);  // Move to next character
            continue;
        }

        // 5. HANDLE STRING LITERALS (starting with ") [cite: 32]
        if (c == '"') {
            cout << "STRING_LITERAL: \"";
            c = fgetc(myfile);  // Skip the opening quote

            // Read until we find the closing quote or end of file
            while (c != EOF && c != '"') {
                cout << c;
                c = fgetc(myfile);
            }

            // Print the closing quote if we found it
            if (c == '"') {
                cout << "\"" << endl;
                c = fgetc(myfile);  // Consume the closing quote
            }
            continue;
        }

        // 6. HANDLE NUMBERS [cite: 32]
        if (isDigit(c)) {
            cout << "NUMBER: ";
            // Keep reading as long as the characters are digits
            while (c != EOF && isDigit(c)) {
                cout << c;
                c = fgetc(myfile);
            }
            cout << endl;
            continue;
        }

        // 7. HANDLE KEYWORDS AND IDENTIFIERS [cite: 30, 31]
        // Identifiers and keywords always start with a letter or underscore
        if (isAlpha(c)) {
            char buffer[256];  // Static char array to hold the token
            int idx = 0;

            // Keep reading alphanumeric characters into the buffer
            while (c != EOF && isAlnum(c) && idx < 255) {
                buffer[idx++] = c;
                c = fgetc(myfile);
            }
            buffer[idx] = '\0';  // Null-terminate the string

            // Check if the buffered word matches any of our target keywords
            if (strEquals(buffer, "int") || strEquals(buffer, "double") ||
                strEquals(buffer, "return")) {
                cout << "KEYWORD: " << buffer << endl;
            } else {
                // If it's not a keyword, it must be an identifier (like a
                // variable name)
                cout << "IDENTIFIER: " << buffer << endl;
            }
            continue;
        }

        // 8. CATCH-ALL FOR UNKNOWN CHARACTERS
        cout << "UNKNOWN: " << c << endl;
        c = fgetc(myfile);
    }

    cout << "--- Tokenization Complete ---" << endl;

    // Close the file to free up system resources
    fclose(myfile);
}

int main() {
    double a, b, product;
    product = a * b;
    return 0;
}