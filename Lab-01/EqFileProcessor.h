#ifndef EQFILEPROCESSOR
#define EQFILEPROCESSOR

#include <ostream>
#include <string>

#include "Stack.h"

using namespace std;

class EqFileProcessor {
    // Name: Ahmed Yar
    // CMS-ID: 480756
    // Semester: 6th
    // Course: Compile Construction
    // Instructor: Dr. Adnan Idrees
    // Lab-Engineer: Mr Safder Ali

   private:
    int getPrecedence(char op);
    int applyOp(int a, int b, char op, bool& error);
    void evaluate(const char* tokens, ostream& output);

   public:
    void run(const char* inputPath, const char* outputPath);
};

#endif