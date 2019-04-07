#include <stdio.h>
#include <stdlib.h>
//ungetc : pushes the character char (an unsigned char) onto the specified stream so that the this is available for the next read operation.
double expression(void);

double vars[26]; // variables
// get one byte
char get(void) {
   char c = getchar();
   return c;
 }
// nextByte at next byte
char nextByte(void) {
  char c = getchar();
   ungetc(c, stdin);
   return c;
  }
// read one double
double readNumber(void) {
   double d;
   scanf("%lf", &d);
   return d; }
// expect char c from stream
void expectStream(char c) {
    char d = get();
    if (c != d) { //in case something goes wrong
        fprintf(stderr, "Error: Expected %c but got %c.\n", c, d);
    }
}
// read a factor
double element(void) {
    double f;
    char c = nextByte();
    if (c == '(') { // an expression inside parantesis?
        expectStream('(');
        f = expression();
        expectStream(')');
    }
    else if (c >= 'A' && c <= 'Z') { // a variable ?
        expectStream(c);
        f = vars[c - 'A'];
    }
    else if (c >= 'a' && c <= 'z') { // a variable ?
        expectStream(c);
        f = vars[c - 'a'];
    }

    else { // or a number?
        f = readNumber();
    }
    return f;
}
// read a term
double term(void) {
    double t = element();
    while (nextByte() == '*' || nextByte() == '/') { // * or / more factors
        char c = get();
        if (c == '*') {
            t *= element();
        } else {
            t /= element();
        }
    }
    return t;
}

double expression(void) { // read an expression
    double e = term();
    while (nextByte() == '+' || nextByte() == '-') { // + or - more terms
        char c = get();
        if (c == '+') {
            e += term();
        }
        else {
            e -= term();
        }
    }
    return e;
}

double statement(void) { // read a statement
    double ret;
    char c = nextByte();
    if (c >= 'A' && c <= 'Z') { // variable ?
        expectStream(c);
        if (nextByte() == '=') { // assignment ?
            expectStream('=');
            double val = expression();
            vars[c - 'A'] = val;
            ret = val;
        }
        else {
            ungetc(c, stdin);
            ret = expression();
        }
    }
    else if (c >= 'a' && c <= 'z') { // variable ?
        expectStream(c);
        if (nextByte() == '=') { // assignment ?
            expectStream('=');
            double val = expression();
            vars[c - 'a'] = val;
            ret = val;
        }
        else {
            ungetc(c, stdin);
            ret = expression();
        }
    }

    else {
        ret = expression();
    }
    expectStream('\n');
    return ret;
}

int main(void) {
    printf(">> "); fflush(stdout);

    for (;;) {
        double v = statement();
        printf("%lf\n>> ", v); fflush(stdout);
    }

    return EXIT_SUCCESS;
}

