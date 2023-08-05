#include "help_text_ana.h"
#include "structs.h"
#include "secondPass.h"
#include "printFunct.h"

/*Errors for: label definition. */
#define symbol_length_error "An error occurred while setting the label. The max length of a label should be 31 characters"
#define symbol_not_alpha_error "An error occurred while setting the label. A label must start with a letter (small or large)"
#define symbol_alp_num_error "An error occurred while setting the label. All characters must be letters or numbers"
#define symbol_exist_error "An error occured while setting the label. This label already defined"
#define symbol_reserved_error "An error occured while setting the label. Reserved words of an assembly language cannot be used as a label name"

Macro_node* macros_list = NULL;
Symbol_node* symbols_list = NULL;

int is_label(char*, int, int, int);
int insert_string(char*, int);
int insert_data(char*, int);
int* init_att(int, int, int, int);
instructions_types get_data_type(char*);
addressing_modes get_add_mode(char*, int, int, int, int, int, int, char*, FILE*);
int process_operands(char*, opcode, funct, int, int, char*, FILE*);
int handle_macros(FILE*, FILE*);