#ifndef EQFILEPROCESSOR
#define EQFILEPROCESSOR

#include <string>
#include <ostream>
#include "Stack.h"


using namespace std;

class EqFileProcessor {
private:
    int getPrecedence(char op);
    int applyOp(int a, int b, char op, bool& error);
    void evaluate(const char* tokens, ostream& output);

public:
    void run(const char* inputPath, const char* outputPath);
};

#endif