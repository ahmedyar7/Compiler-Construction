#ifndef RECURSIVEDESCENT_H
#define RECURSIVEDESCENT_H

#include <fstream>

// enum NodeType { NODE_OP, NODE_ID, NODE_NUM };

// struct Node {
//     NodeType type;
//     char value[32]; // To store "x" or "23"
//     Node *left, *right;

//     // Constructor for easy creation
//     Node(NodeType t, const char* v) : type(t), left(nullptr), right(nullptr)
//     {
//         int i = 0;
//         while (v[i] != '\0' && i < 31) {
//             value[i] = v[i];
//             i++;
//         }
//         value[i] = '\0';
//     }
// };

class RecursiveDescent {
   private:
    const char* input;
    int pos;
    bool hasError;

    void skipWhitespace();
    char peek();
    char get();

    // Grammar rules
    void parseE();
    void parseT();
    void parseP();
    void parseF();

   public:
    RecursiveDescent();
    void parseExpression(const char* expression);
};

#endif