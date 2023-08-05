#include <ctype.h>
#include <string.h>
#include <math.h>
#include "shared.h"

char* skip_whitespaces(char*);
void remove_whitespaces(char*);
int word_length(char*);
char* get_first_word(char*);
char* get_next_word(char*);
int get_num(char*, int);
int end_is_clear(char*);
int is_there_bracket(char*);
char* strallocat(char*, char*);