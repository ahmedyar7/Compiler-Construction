#include <iostream>
#include <string>
#include <cctype>
#include <map>
using namespace std;

string expr;
int pos = 0;
map<string, double> variables;

// ---- TOKEN SYSTEM ----
string token;
enum TokenType { PLUS, MUL, LPAREN, RPAREN, NUMBER, ID, END, INVALID };
TokenType currentType;

void getToken() {
    while (pos < expr.length() && expr[pos] == ' ') pos++;

    if (pos >= expr.length()) {
        currentType = END;
        token = "";
        return;
    }

    char c = expr[pos];

    if (c == '+') { currentType = PLUS; token = "+"; pos++; return; }
    if (c == '*') { currentType = MUL; token = "*"; pos++; return; }
    if (c == '(') { currentType = LPAREN; token = "("; pos++; return; }
    if (c == ')') { currentType = RPAREN; token = ")"; pos++; return; }

    if (isdigit(c)) {
        string num = "";
        while (pos < expr.length() && (isdigit(expr[pos]) || expr[pos] == '.'))
            num += expr[pos++];
        token = num;
        currentType = NUMBER;
        return;
    }

    if (isalpha(c)) {
        string id = "";
        while (pos < expr.length() && isalnum(expr[pos]))
            id += expr[pos++];
        token = id;
        currentType = ID;
        return;
    }

    currentType = INVALID;
}

// ---- PARSER FUNCTIONS (with step tracing) ----

double E(); double T(); double F();

// T → F { * F }
double T() {
    cout << "Enter T\n";

    double value = F();

    while (currentType == MUL) {
        cout << "Matched *\n";
        getToken();
        value *= F();
    }

    cout << "Exit T with value: " << value << "\n";
    return value;
}

// E → T { + T }
double E() {
    cout << "Enter E\n";

    double value = T();

    while (currentType == PLUS) {
        cout << "Matched +\n";
        getToken();
        value += T();
    }

    cout << "Exit E with value: " << value << "\n";
    return value;
}

// F → (E) | number | id
double F() {
    cout << "Enter F\n";

    if (currentType == NUMBER) {
        cout << "Matched NUMBER: " << token << "\n";
        double val = stod(token);
        getToken();
        cout << "Exit F with number: " << val << "\n";
        return val;
    }

    else if (currentType == ID) {
        cout << "Matched ID: " << token << "\n";
        string id = token;
        getToken();

        if (variables.find(id) == variables.end()) {
            cout << "Enter value for " << id << ": ";
            double v;
            cin >> v;
            variables[id] = v;
        }

        cout << "Exit F with ID value: " << variables[id] << "\n";
        return variables[id];
    }

    else if (currentType == LPAREN) {
        cout << "Matched (\n";
        getToken();
        double val = E();
        if (currentType != RPAREN) {
            cout << "Syntax Error: Missing )\n";
            exit(0);
        }
        cout << "Matched )\n";
        getToken();
        cout << "Exit F with parenthesis value: " << val << "\n";
        return val;
    }

    else {
        cout << "Syntax Error in Factor\n";
        exit(0);
    }
}

int main() {
    cout << "Enter expression: ";
    getline(cin, expr);

    pos = 0;
    getToken();

    double result = E();

    if (currentType != END) {
        cout << "Syntax Error: Extra characters at end\n";
        return 0;
    }

    cout << "\nFINAL OUTPUT:\n";
    cout << "(" << expr << ") = " << result << endl;

    return 0;
}
