#include "EqFileProcessor.h"
#include "EqFileProcessor.cpp"
using namespace std;

// Name: Ahmed Yar
// CMS-ID: 480756
// Semester: 6th 
// Course: Compile Construction
// Instructor: Dr. Adnan Idrees 
// Lab-Engineer: Mr Safder Ali

int main() {
    EqFileProcessor processor;
    processor.run("equations.txt", "answers.txt");
    return 0;
}