# include<iostream>

using namespace std;

char inputBuff[100];
int pos = 0;

char peek(){return inputBuff[pos];}
char get(){return inputBuff[pos++];}
void error(const char *msg){throw msg;}

double expression();
double factor();
double term();

double number(){

    double result = 0.0;
    double fraction = 1.0;

    bool isFraction = false;
    bool hasDigital = false;

    // parse integer part
    while(isdigit(peek())){

        result = result * 10 + (get() - '0');
        hasDigital = true;
    }


    if(pe)
}


int main(){

    return 0;
}