#ifndef CODE_INDENTOR_H
#define CODE_INDENTOR_H

#include <fstream>

class CodeIndentor {
   private:
    int m_indentLevel;

    // Helper functions
    void printIndentation(std::ofstream& outFile, int level);
    bool isEmpty(char* line);

   public:
    CodeIndentor();
    void processInputFile(const char* inputFileName,
                          const char* outputFileName);
};

#endif