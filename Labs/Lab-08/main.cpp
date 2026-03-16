#include <iostream>
using namespace std;

// ── Data Types ────────────────────────────────
enum DataType { INT, DOUBLE, STRING };

// ── Symbol Structure ──────────────────────────
struct Symbol {
    char name[64];
    DataType type;
    int      int_val;
    double   dbl_val;
    char     str_val[256];
};

// ── Global Symbol Table ───────────────────────
Symbol table[50];
int tableSize = 0;

// ── Helper Functions ──────────────────────────
void copyStr(char* dst, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

bool strEqual(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

const char* typeName(DataType t) {
    if (t == INT)    return "int";
    if (t == DOUBLE) return "double";
    return "string";
}

// ── Symbol Table Functions ────────────────────
int findSymbol(const char* name) {
    for (int i = 0; i < tableSize; i++) {
        if (strEqual(table[i].name, name))
            return i;
    }
    return -1;
}

void declareSymbol(const char* name, DataType type) {
    if (findSymbol(name) != -1) {
        cout << "Error: '" << name << "' is already declared." << endl;
        return;
    }
    copyStr(table[tableSize].name, name);
    table[tableSize].type    = type;
    table[tableSize].int_val = 0;
    table[tableSize].dbl_val = 0.0;
    table[tableSize].str_val[0] = '\0';
    tableSize++;
    cout << "Declared: " << name << " --> " << typeName(type) << endl;
}

void assignInt(const char* name, int val) {
    int i = findSymbol(name);
    if (i == -1) {
        cout << "Error: '" << name << "' was not declared." << endl;
        return;
    }
    if (table[i].type != INT) {
        cout << "Semantic Error: Cannot assign int to '"
             << name << "' which is a " << typeName(table[i].type) << endl;
        return;
    }
    table[i].int_val = val;
    cout << "Assigned: " << name << " = " << val << endl;
}

void assignDouble(const char* name, double val) {
    int i = findSymbol(name);
    if (i == -1) {
        cout << "Error: '" << name << "' was not declared." << endl;
        return;
    }
    if (table[i].type != DOUBLE) {
        cout << "Semantic Error: Cannot assign double to '"
             << name << "' which is a " << typeName(table[i].type) << endl;
        return;
    }
    table[i].dbl_val = val;
    cout << "Assigned: " << name << " = " << val << endl;
}

void assignString(const char* name, const char* val) {
    int i = findSymbol(name);
    if (i == -1) {
        cout << "Error: '" << name << "' was not declared." << endl;
        return;
    }
    if (table[i].type != STRING) {
        cout << "Semantic Error: Cannot assign string to '"
             << name << "' which is a " << typeName(table[i].type) << endl;
        return;
    }
    copyStr(table[i].str_val, val);
    cout << "Assigned: " << name << " = \"" << val << "\"" << endl;
}

void printTable() {
    cout << "\n======= Symbol Table =======" << endl;
    cout << "Name       Type      Value" << endl;
    cout << "----------------------------" << endl;
    for (int i = 0; i < tableSize; i++) {
        cout << table[i].name << "        "
             << typeName(table[i].type) << "      ";
        if (table[i].type == INT)
            cout << table[i].int_val << endl;
        else if (table[i].type == DOUBLE)
            cout << table[i].dbl_val << endl;
        else
            cout << "\"" << table[i].str_val << "\"" << endl;
    }
    cout << "============================\n" << endl;
}

// ── Type Checker ──────────────────────────────
void checkAddition(const char* left, const char* right) {
    cout << "Expression: " << left << " + " << right << endl;

    if (strEqual(left, "int") && strEqual(right, "int")) {
        cout << "Result: int  (OK)\n" << endl;
        return;
    }

    if (strEqual(left, "double") && strEqual(right, "double")) {
        cout << "Result: double  (OK)\n" << endl;
        return;
    }

    if ((strEqual(left, "int")    && strEqual(right, "double")) ||
        (strEqual(left, "double") && strEqual(right, "int"))) {
        cout << "Result: double  (int gets promoted to double — OK)\n" << endl;
        return;
    }

    if (strEqual(left, "string") && strEqual(right, "string")) {
        cout << "Result: string  (string concatenation — OK)\n" << endl;
        return;
    }

    cout << "SEMANTIC ERROR: Type Mismatch!" << endl;
    cout << "Cannot add '" << left << "' and '" << right << "'" << endl;
    cout << "This is a semantic error NOT a syntax error because" << endl;
    cout << "the structure (expr + expr) looks fine to the parser" << endl;
    cout << "but the types are logically incompatible.\n" << endl;
}

// ─────────────────────────────────────────────
int main() {
    cout << "===== CS-354 Lab 08: Type Systems =====\n" << endl;

    // ── Task 1: Declare variables
    cout << "--- Task 1: Declaring Variables ---" << endl;
    declareSymbol("age",    INT);
    declareSymbol("gpa",    DOUBLE);
    declareSymbol("myVar",  STRING);

    // ── Task 4: Assign values
    cout << "\n--- Task 4: Assigning Values ---" << endl;
    assignInt   ("age",   21);
    assignDouble("gpa",   3.9);
    assignString("myVar", "NUST");

    // ── Task 4: Illegal reassignments
    cout << "\n--- Task 4: Illegal Reassignments ---" << endl;
    assignInt   ("myVar", 100);       // string = int    --> Semantic Error
    assignString("age",   "hello");   // int    = string --> Semantic Error

    // ── Print Symbol Table
    printTable();

    // ── Task 2 & 3: Type Checker
    cout << "--- Task 2 & 3: Type Checker ---\n" << endl;
    checkAddition("int",    "int");
    checkAddition("double", "double");
    checkAddition("int",    "double");
    checkAddition("double", "int");
    checkAddition("string", "string");
    checkAddition("int",    "string");    // 42 + "grapefruit"
    checkAddition("string", "int");       // "test" + 10
    checkAddition("string", "double");    // "hi" + 3.14

    return 0;
}