#include "printFunct.h"

void print_symbols(Symbol_node** head)
{
    Symbol_node* last = *head;
    printf("Start symbols\n");
    while (last->next != NULL)
    {
        printf("symbol: %s, value %d, bass %d, offest %d, att: %d %d %d %d\n", last->name, last->value, last->base_address, last->offset, last->attributes[0], last->attributes[1], last->attributes[2], last->attributes[3]);
        last = last->next;
    }
    printf("End symbols\n");
}


/*
* Checks how long is the name of the given file.
* @param fileName The name of the given file
* @return Returns the last index of the given file name.
*/
int howLong(char* fileName) {
    int i;
    for (i = 0; i < 100; i++) {
        if (fileName[i] == '\0') {
            return i;
        }
    }
    return -1;
}


/*
* Prints the base address and the offset of the entry symbols.
* @param symbols_list A pointer to the head of the symbols linked list.
* @param pharse The name of the symbol.
* @param ps_ent The output file of the entry symbols.
* @param fileName The name of the given file.
*/
void printEntry(Symbol_node* symbols_list, FILE* ps_ent, char* fileName) {

    Symbol_node* current = symbols_list;
    int currentIndex;
    char newFileNamme[104];

    while (current != NULL)
    {
        if (get_attributes(symbols_list, current->name)[2] == 1) {
            if (ps_ent == NULL) {
                currentIndex = howLong(fileName);
                strcpy(newFileNamme, fileName);
                newFileNamme[currentIndex] = '.';
                newFileNamme[currentIndex + 1] = 'e';
                newFileNamme[currentIndex + 2] = 'n';
                newFileNamme[currentIndex + 3] = 't';
                newFileNamme[currentIndex + 4] = '\0';
                ps_ent = fopen(newFileNamme, "w");
            }
            fprintf(ps_ent, "%s,%d,%d \n", current->name, get_base_address(symbols_list, current->name), get_offset(symbols_list, current->name));
        }
        current = current->next;
    }
}

/*
* Prints the address of the data words that need the bass address and the offset of the external symbols.
* @param symbols_list A pointer to the head of the symbols linked list.
* @param pharse The name of the symbol.
* @param ps_ext The output file of the external symbols.
* @param dc The number of DC after puting the data words of the external symbol in the machine code.
* @param fileName The name of the given file.
* @return Returns the output file of the external symbols.
*/
FILE* printExtern(Symbol_node* symbols_list, char* pharse, FILE* ps_ext, int ic, char* fileName) {
    int* attr = get_attributes(symbols_list, pharse);
    if (attr == NULL) {
        return ps_ext;

    }

    if (attr[3]) {
        fprintf(ps_ext, "%s BASE %d \n", pharse, ic - 2);
        fprintf(ps_ext, "%s OFFSET %d \n \n", pharse, ic - 1);
    }

    return ps_ext;
}

/*
* Fills a line in the machine code array.
* @param newSentence The current line in the machine code array.
* @param j The index in the current line.
* @param n The number  in decimal of a few bytes in the current line.
* @param size The amount of the few bytes in the current line.
* @return Returns the index in the current line.
*/
int fillArray(int newSentence[20], int j, int n, int size) {
    while (n > 0) {
        newSentence[j] = n % 2;
        n = n / 2;
        j++;
    }
    while (j < size) {
        newSentence[j] = 0;
        j++;
    }
    return j;
}

/*
* Prints the output file of the machine code.
* @param fileName The name of the given file.
*/
void printFiles(char* fileName, int commands) {
    int outPutIndex = howLong(fileName);
    char outPutName[103];
    FILE* fp;
    int i = 0;

    strcpy(outPutName, fileName);
    outPutName[outPutIndex] = '.';
    outPutName[outPutIndex + 1] = 'o';
    outPutName[outPutIndex + 2] = 'b';
    outPutName[outPutIndex + 3] = '\0';

    fp = fopen(outPutName, "w");

    fprintf(fp, "       %d  %d\n", IC -100 - DC, DC);

    for (i = 100; i < IC; i++) {
        if (i < 1000) {
            fprintf(fp, "0");
        }

        fprintf(fp, "%d ", i);

        if (mc[i].dt != NULL && mc[i].dt->exists == 1) {
            printData(mc[i].dt, fp);

        }
        else if (mc[i].cd != NULL && mc[i].cd->exists == 2) {
            printCode(mc[i].cd, fp);
        }
        else {
            printf("nothing\n");
        }

        fprintf(fp, "\n");
    }
}

/*
* Prints the code words to the output file of the machine code.
* @param cd The current code word.
* @param fp The output file of the machine code.
*/
void printCode(code_word* cd, FILE* fp) {
    int j, k, n, value, newSentence[20];

    n = cd->destination_addressing;
    j = 0;
    j = fillArray(newSentence, j, n, 2);

    n = cd->destination_register;
    j = fillArray(newSentence, j, n, 6);

    n = cd->source_addressing;
    j = fillArray(newSentence, j, n, 8);

    n = cd->source_register;
    j = fillArray(newSentence, j, n, 12);

    n = cd->funct;
    j = fillArray(newSentence, j, n, 16);

    n = cd->A_R_E;
    j = fillArray(newSentence, j, n, 19);

    newSentence[j] = 0;

    for (k = 4; k > -1; k--) {
        value = newSentence[k * 4 + 3] * 8 + newSentence[k * 4 + 2] * 4 + newSentence[k * 4 + 1] * 2 + newSentence[k * 4] * 1;
        printLetter(k, fp);
        printNumber(value, fp);
    }
}

/*
* Prints the data words to the output file of the machine code.
* @param dt The current data word.
* @param fp The output file of the machine code.
*/
void printData(data_word* dt, FILE* fp) {
    int j, k, n, value, newSentence[20];

    j = 0;
    n = dt->data;
    j = fillArray(newSentence, j, n, 16);

    n = dt->A_R_E;
    j = fillArray(newSentence, j, n, 19);

    newSentence[j] = 0;


    for (k = 4; k > -1; k--) {
        value = newSentence[k * 4 + 3] * 8 + newSentence[k * 4 + 2] * 4 + newSentence[k * 4 + 1] * 2 + newSentence[k * 4] * 1;
        printLetter(k, fp);
        printNumber(value, fp);
    }

    /*for(j = 19; j > -1; j--){
         printf("%d ", newSentence[j]);
    }
    printf("\n");*/

}

/*
* Prints the number in hexademical to the output file of the machine code.
* @param num The current number in decimal.
* @param fp The output file of the machine code.
*/
void printNumber(int num, FILE* fp) {
    if (num < 10) {
        fprintf(fp, "%d", num);
    }
    else if (num == 10) {
        fprintf(fp, "a");
    }
    else if (num == 11) {
        fprintf(fp, "b");
    }
    else if (num == 12) {
        fprintf(fp, "c");
    }
    else if (num == 13) {
        fprintf(fp, "d");
    }
    else if (num == 14) {
        fprintf(fp, "e");
    }
    else if (num == 15) {
        fprintf(fp, "f");
    }
}

/*
* Prints the current letter to the output file of the machine code.
* @param k The index that decides which letter should be now.
* @param fp The output file of the machine code.
*/
void printLetter(int k, FILE* fp) {
    if (k == 4) {
        fprintf(fp, "A");
    }
    else if (k == 3) {
        fprintf(fp, "-B");
    }
    else if (k == 2) {
        fprintf(fp, "-C");
    }
    else if (k == 1) {
        fprintf(fp, "-D");
    }
    else if (k == 0) {
        fprintf(fp, "-E");
    }
}