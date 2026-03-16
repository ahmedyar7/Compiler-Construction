#include <stdio.h>
#include <stdlib.h>

// ─────────────────────────────────────────────
// UTILITIES (no <string.h> — manual char ops)
// ─────────────────────────────────────────────
int my_strlen(const char* s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

void my_strcpy(char* dst, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

int my_strcmp(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

void my_strcat(char* dst, const char* src) {
    int d = my_strlen(dst);
    int i = 0;
    while (src[i] != '\0') {
        dst[d + i] = src[i];
        i++;
    }
    dst[d + i] = '\0';
}

void int_to_str(int n, char* buf) {
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    int neg = 0, i = 0;
    char tmp[32];
    if (n < 0) {
        neg = 1;
        n = -n;
    }
    while (n > 0) {
        tmp[i++] = '0' + (n % 10);
        n /= 10;
    }
    if (neg) tmp[i++] = '-';
    // reverse
    int j = 0;
    for (int k = i - 1; k >= 0; k--) buf[j++] = tmp[k];
    buf[j] = '\0';
}

// ─────────────────────────────────────────────
// TASK 1 — DataType enum & Symbol Table
// ─────────────────────────────────────────────
enum DataType { INT, DOUBLE, STRING, UNKNOWN };

const char* dtype_name(DataType d) {
    if (d == INT) return "INT";
    if (d == DOUBLE) return "DOUBLE";
    if (d == STRING) return "STRING";
    return "UNKNOWN";
}

struct SymbolEntry {
    char name[64];
    DataType type;
    int int_val;
    double dbl_val;
    char str_val[256];
    int is_set;  // 1 = has a value
};

#define MAX_SYMBOLS 64
struct SymbolTable {
    SymbolEntry entries[MAX_SYMBOLS];
    int count;
};

void st_init(SymbolTable* st) { st->count = 0; }

// Returns index or -1
int st_find(SymbolTable* st, const char* name) {
    for (int i = 0; i < st->count; i++)
        if (my_strcmp(st->entries[i].name, name) == 0) return i;
    return -1;
}

// Declare a new variable; returns index or -1 on overflow / redeclaration
int st_declare(SymbolTable* st, const char* name, DataType type) {
    if (st_find(st, name) != -1) {
        printf("[Symbol Table] ERROR: '%s' already declared.\n", name);
        return -1;
    }
    if (st->count >= MAX_SYMBOLS) {
        printf("[Symbol Table] ERROR: table full.\n");
        return -1;
    }
    int i = st->count++;
    my_strcpy(st->entries[i].name, name);
    st->entries[i].type = type;
    st->entries[i].is_set = 0;
    return i;
}

void st_print(SymbolTable* st) {
    printf("\n┌──────────────────────────────────────────────┐\n");
    printf("│              SYMBOL TABLE                    │\n");
    printf("├──────────────┬──────────┬────────────────────┤\n");
    printf("│ Name         │ Type     │ Value              │\n");
    printf("├──────────────┼──────────┼────────────────────┤\n");
    for (int i = 0; i < st->count; i++) {
        SymbolEntry* e = &st->entries[i];
        char val[64] = "<unset>";
        if (e->is_set) {
            if (e->type == INT) int_to_str(e->int_val, val);
            if (e->type == DOUBLE) {
                // simple double print via sprintf is OK (no extra library)
                sprintf(val, "%.4f", e->dbl_val);
            }
            if (e->type == STRING) {
                val[0] = '"';
                my_strcpy(val + 1, e->str_val);
                int l = my_strlen(val);
                val[l] = '"';
                val[l + 1] = '\0';
            }
        }
        printf("│ %-12s │ %-8s │ %-18s │\n", e->name, dtype_name(e->type), val);
    }
    printf("└──────────────┴──────────┴────────────────────┘\n\n");
}

// ─────────────────────────────────────────────
// TASK 2 & 3 — Type Checker & Expression Engine
// ─────────────────────────────────────────────

// Represents one operand value during expression evaluation
struct Value {
    DataType type;
    int int_val;
    double dbl_val;
    char str_val[256];
    int valid;  // 0 = error already reported
};

Value make_int(int v) {
    Value r;
    r.type = INT;
    r.int_val = v;
    r.valid = 1;
    return r;
}
Value make_double(double v) {
    Value r;
    r.type = DOUBLE;
    r.dbl_val = v;
    r.valid = 1;
    return r;
}
Value make_str(const char* s) {
    Value r;
    r.type = STRING;
    my_strcpy(r.str_val, s);
    r.valid = 1;
    return r;
}
Value make_error() {
    Value r;
    r.type = UNKNOWN;
    r.valid = 0;
    return r;
}

void print_value(Value v) {
    if (!v.valid) {
        printf("<error>");
        return;
    }
    if (v.type == INT) printf("%d (INT)", v.int_val);
    if (v.type == DOUBLE) printf("%.4f (DOUBLE)", v.dbl_val);
    if (v.type == STRING) printf("\"%s\" (STRING)", v.str_val);
}

// ── Type Checker ──────────────────────────────
// Task 2: checks operator legality; Task 3: applies promotion rules
Value type_check_add(Value left, Value right) {
    if (!left.valid || !right.valid) return make_error();

    // int + int → int
    if (left.type == INT && right.type == INT) {
        printf("[Type Checker] int + int → int  (OK)\n");
        return make_int(left.int_val + right.int_val);
    }
    // double + double → double
    if (left.type == DOUBLE && right.type == DOUBLE) {
        printf("[Type Checker] double + double → double  (OK)\n");
        return make_double(left.dbl_val + right.dbl_val);
    }
    // int + double  OR  double + int → PROMOTION to double
    if ((left.type == INT && right.type == DOUBLE) ||
        (left.type == DOUBLE && right.type == INT)) {
        double l = (left.type == INT) ? (double)left.int_val : left.dbl_val;
        double r = (right.type == INT) ? (double)right.int_val : right.dbl_val;
        printf(
            "[Type Checker] int + double → PROMOTION: int promoted to double  "
            "(OK)\n");
        return make_double(l + r);
    }
    // string + string → concatenation
    if (left.type == STRING && right.type == STRING) {
        printf("[Type Checker] string + string → STRING concatenation  (OK)\n");
        char buf[512];
        my_strcpy(buf, left.str_val);
        my_strcat(buf, right.str_val);
        return make_str(buf);
    }
    // string + int  OR  int + string → SEMANTIC ERROR (no implicit cast)
    if ((left.type == STRING && right.type == INT) ||
        (left.type == INT && right.type == STRING)) {
        printf(
            "[Type Checker] *** TYPE MISMATCH ERROR: "
            "Cannot add STRING and INT. "
            "(Implicit casting is NOT performed — "
            "this is a semantic error, not a syntax error.)\n");
        return make_error();
    }
    // string + double  OR  double + string → SEMANTIC ERROR
    if ((left.type == STRING && right.type == DOUBLE) ||
        (left.type == DOUBLE && right.type == STRING)) {
        printf(
            "[Type Checker] *** TYPE MISMATCH ERROR: "
            "Cannot add STRING and DOUBLE.\n");
        return make_error();
    }
    // catch-all
    printf(
        "[Type Checker] *** TYPE MISMATCH ERROR: "
        "Unsupported types %s + %s\n",
        dtype_name(left.type), dtype_name(right.type));
    return make_error();
}

// ─────────────────────────────────────────────
// TASK 4 — String Assignment & Reassignment Check
// ─────────────────────────────────────────────
int st_assign_int(SymbolTable* st, const char* name, int val) {
    int i = st_find(st, name);
    if (i == -1) {
        printf("[Symbol Table] ERROR: '%s' not declared.\n", name);
        return 0;
    }
    if (st->entries[i].type != INT) {
        printf(
            "[Semantic Error] ASSIGNMENT TYPE MISMATCH: "
            "'%s' is declared as %s but you tried to assign INT.\n",
            name, dtype_name(st->entries[i].type));
        return 0;
    }
    st->entries[i].int_val = val;
    st->entries[i].is_set = 1;
    return 1;
}

int st_assign_double(SymbolTable* st, const char* name, double val) {
    int i = st_find(st, name);
    if (i == -1) {
        printf("[Symbol Table] ERROR: '%s' not declared.\n", name);
        return 0;
    }
    if (st->entries[i].type != DOUBLE) {
        // allow int = double? We treat as error (strict mode)
        printf(
            "[Semantic Error] ASSIGNMENT TYPE MISMATCH: "
            "'%s' is declared as %s but you tried to assign DOUBLE.\n",
            name, dtype_name(st->entries[i].type));
        return 0;
    }
    st->entries[i].dbl_val = val;
    st->entries[i].is_set = 1;
    return 1;
}

int st_assign_string(SymbolTable* st, const char* name, const char* val) {
    int i = st_find(st, name);
    if (i == -1) {
        printf("[Symbol Table] ERROR: '%s' not declared.\n", name);
        return 0;
    }
    if (st->entries[i].type != STRING) {
        printf(
            "[Semantic Error] ASSIGNMENT TYPE MISMATCH: "
            "'%s' is declared as %s but you tried to assign STRING.\n",
            name, dtype_name(st->entries[i].type));
        return 0;
    }
    my_strcpy(st->entries[i].str_val, val);
    st->entries[i].is_set = 1;
    return 1;
}

// ─────────────────────────────────────────────
// MINI SCANNER (Task 1) — tokenises a line
// ─────────────────────────────────────────────
enum TokenType {
    TOK_INT_KW,
    TOK_DOUBLE_KW,
    TOK_STRING_KW,
    TOK_INT_LIT,
    TOK_DOUBLE_LIT,
    TOK_STRING_LIT,
    TOK_IDENT,
    TOK_PLUS,
    TOK_ASSIGN,
    TOK_SEMI,
    TOK_EOF,
    TOK_UNKNOWN
};

struct Token {
    TokenType type;
    char lexeme[256];
    int int_val;
    double dbl_val;
};

// Whitespace skip
int is_space(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }
int is_digit(char c) { return c >= '0' && c <= '9'; }
int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
int is_alnum(char c) { return is_alpha(c) || is_digit(c); }

// Scan one token from src starting at *pos
Token scan_token(const char* src, int* pos) {
    Token tok;
    tok.lexeme[0] = '\0';
    // skip whitespace
    while (src[*pos] && is_space(src[*pos])) (*pos)++;
    if (!src[*pos]) {
        tok.type = TOK_EOF;
        return tok;
    }

    char c = src[*pos];

    // String literal
    if (c == '"') {
        (*pos)++;
        int i = 0;
        while (src[*pos] && src[*pos] != '"') tok.lexeme[i++] = src[(*pos)++];
        tok.lexeme[i] = '\0';
        if (src[*pos] == '"') (*pos)++;
        tok.type = TOK_STRING_LIT;
        return tok;
    }
    // Number
    if (is_digit(c) || (c == '-' && is_digit(src[*pos + 1]))) {
        int i = 0;
        int has_dot = 0;
        if (c == '-') tok.lexeme[i++] = src[(*pos)++];
        while (src[*pos] && (is_digit(src[*pos]) || src[*pos] == '.')) {
            if (src[*pos] == '.') has_dot = 1;
            tok.lexeme[i++] = src[(*pos)++];
        }
        tok.lexeme[i] = '\0';
        if (has_dot) {
            tok.type = TOK_DOUBLE_LIT;
            tok.dbl_val = atof(tok.lexeme);
        } else {
            tok.type = TOK_INT_LIT;
            tok.int_val = atoi(tok.lexeme);
        }
        return tok;
    }
    // Identifier or keyword
    if (is_alpha(c)) {
        int i = 0;
        while (src[*pos] && is_alnum(src[*pos]))
            tok.lexeme[i++] = src[(*pos)++];
        tok.lexeme[i] = '\0';
        if (my_strcmp(tok.lexeme, "int") == 0)
            tok.type = TOK_INT_KW;
        else if (my_strcmp(tok.lexeme, "double") == 0)
            tok.type = TOK_DOUBLE_KW;
        else if (my_strcmp(tok.lexeme, "string") == 0)
            tok.type = TOK_STRING_KW;
        else
            tok.type = TOK_IDENT;
        return tok;
    }
    // Single-char operators
    tok.lexeme[0] = c;
    tok.lexeme[1] = '\0';
    (*pos)++;
    if (c == '+')
        tok.type = TOK_PLUS;
    else if (c == '=')
        tok.type = TOK_ASSIGN;
    else if (c == ';')
        tok.type = TOK_SEMI;
    else
        tok.type = TOK_UNKNOWN;
    return tok;
}

// ─────────────────────────────────────────────
// DEMO — runs all four tasks
// ─────────────────────────────────────────────
int main() {
    printf("========================================\n");
    printf("  CS-354 Lab 08 — Type Systems Demo\n");
    printf("========================================\n\n");

    SymbolTable st;
    st_init(&st);

    // ── Task 1: Declare variables ─────────────
    printf("─── TASK 1: Scanner & Symbol Table ────\n");
    const char* decls[] = {"int    age", "double gpa", "string name"};
    DataType dtypes[] = {INT, DOUBLE, STRING};
    for (int d = 0; d < 3; d++) {
        int pos = 0;
        Token kw = scan_token(decls[d], &pos);
        Token idn = scan_token(decls[d], &pos);
        printf("[Scanner] keyword='%s'  identifier='%s'\n", kw.lexeme,
               idn.lexeme);
        st_declare(&st, idn.lexeme, dtypes[d]);
    }

    // ── Task 4: Assign values ─────────────────
    printf("\n─── TASK 4: Assignments ────────────────\n");
    st_assign_int(&st, "age", 21);
    st_assign_double(&st, "gpa", 3.75);
    st_assign_string(&st, "name", "NUST");

    printf("\n[Task 4 — Illegal reassignment] string name = 100 :\n");
    st_assign_int(&st, "name", 100);  // ← should trigger semantic error

    printf("\n[Task 4 — Illegal reassignment] int age = \"hello\" :\n");
    st_assign_string(&st, "age", "hello");  // ← should trigger semantic error

    st_print(&st);

    // ── Task 2 & 3: Type checker ──────────────
    printf("─── TASK 2 & 3: Type Checker ───────────\n\n");

    struct {
        const char* label;
        Value left;
        Value right;
    } cases[] = {
        {"42 + 10        (int + int)", make_int(42), make_int(10)},
        {"4.5 + 2.1      (double + double)", make_double(4.5),
         make_double(2.1)},
        {"4.5 + 2        (double + int)", make_double(4.5), make_int(2)},
        {"2 + 4.5        (int + double)", make_int(2), make_double(4.5)},
        {"\"Hello\" + \" World\" (str+str)", make_str("Hello"),
         make_str(" World")},
        {"42 + \"grapefruit\" (int + str)", make_int(42),
         make_str("grapefruit")},
        {"10 + \"test\"  (int + str)", make_int(10), make_str("test")},
        {"\"hi\" + 3.14  (str + double)", make_str("hi"), make_double(3.14)},
    };

    for (int i = 0; i < 8; i++) {
        printf("Expression: %s\n", cases[i].label);
        printf("  Left  = ");
        print_value(cases[i].left);
        printf("\n");
        printf("  Right = ");
        print_value(cases[i].right);
        printf("\n");
        printf("  ");
        Value result = type_check_add(cases[i].left, cases[i].right);
        if (result.valid) {
            printf("  Result = ");
            print_value(result);
            printf("\n");
        }
        printf("\n");
    }

    printf("========================================\n");
    printf("  All tasks demonstrated successfully.\n");
    printf("========================================\n");
    return 0;
}