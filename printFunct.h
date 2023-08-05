#include "shared.h"
#include "structs.h"

void print_symbols(Symbol_node** head);
int fillArray(int[20], int, int, int);
void printFiles(char*, int);
void printCode(code_word*, FILE*);
void printData(data_word*, FILE*);
void printNumber(int, FILE*);
void printLetter(int, FILE*);
void printEntry(Symbol_node*, FILE*, char*);
FILE* printExtern(Symbol_node*, char*, FILE*, int, char*);
int howLong(char*);
