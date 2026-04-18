#ifndef ADHOCLEXER_H
#define ADHOCLEXER_H

#include <fstream>
#include <iostream>

class AdHocLexer {
   private:
    bool compare(const char* str1, const char* str2);
    bool findKeywords(const char* kw);

   public:
    void lexer(const char* fileName);
};

#endif