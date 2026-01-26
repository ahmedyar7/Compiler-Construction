#include "EqFileProcessor.h"
#include "EqFileProcessor.cpp"
using namespace std;

int main() {
    EqFileProcessor processor;
    processor.run("equations.txt", "answers.txt");
    return 0;
}