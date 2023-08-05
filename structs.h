#ifndef STRUCT_H
#define STRUCT_H
#include "shared.h"
#include "structs.h"

#define SOURCE_OPERAND 0
#define DESTINATION_OPERAND 1

#define mode_A { 1,1,1,1 } /*(0,1,2,3)*/
#define mode_B { 0,1,1,1 } /*(1,2,3)*/
#define mode_C { 0,1,1,0 } /*(1,2)*/
#define mode_D { 0,0,0,0 } /*(None operands)*/

/* -----------------------------------Instructions types---------------------------------------*/
enum instructions_types
{
	DATA_MARK = 0,
	STRING_MARK = 1,
	ENTRY_MARK = 2,
	EXTERN_MARK = 3
} typedef instructions_types;

/* -----------------------------------Macro nodes---------------------------------------*/
struct Macro_node
{
	char* name;
	char* content;
	struct Macro_node* next;
} typedef Macro_node;

Macro_node* add_macro(Macro_node*, char*, char*);
char* search_macro(Macro_node*, char*);

/* -----------------------------------Symbol nodes---------------------------------------*/
struct Symbol_node
{
	char* name;
	int value;
	int base_address;
	int offset;
	int attributes[4]; /* TRUE if yes, FALSE if not: [0] = data, [1] = code, [2] = entry, [3] = external*/
	struct Symbol_node* next;
} typedef Symbol_node;

Symbol_node* add_symbol(Symbol_node**, char*, int, int[]);
int get_base_address(Symbol_node*, char*);
int get_offset(Symbol_node*, char*);
int* get_attributes(Symbol_node*, char*);
int is_extern(Symbol_node*, char*);
int is_symbol_exist(Symbol_node*, char*);
int calc_base_address(int);
int calc_offset(int);

/* ---------------Functons_struct, opecode and funct enums------------------------------*/
enum opcodes_list
{
	mov_opcode = 0,
	cmp_opcode = 1,
	add_opcode = 2,
	sub_opcode = 2,
	lea_opcode = 4,
	clr_opcode = 5,
	not_opcode = 5,
	inc_opcode = 5,
	dec_opcode = 5,
	jmp_opcode = 9,
	bne_opcode = 9,
	jsr_opcode = 9,
	red_opcode = 12,
	prn_opcode = 13,
	rts_opcode = 14,
	stop_opcode = 15
} typedef opcode;

enum operands_num
{
	non_operands = 0,
	one_operand = 1,
	two_operands = 2
} typedef operands_num;

enum funct_list
{
	add_funct = 10,
	sub_funct = 11,
	clr_funct = 10,
	not_funct = 11,
	inc_funct = 12,
	dec_funct = 13,
	jmp_funct = 10,
	bne_funct = 11,
	jsr_funct = 12,
	blank_funct = 0
} typedef funct;

struct Functions
{
	char* name;
	opcode opcode_num;
	funct funct_num;
	operands_num req_operands_num;
	int valid_source_modes[4];
	int valid_dest_modes[4];
} typedef Functions;

int is_function_exist(char*, Functions[], int);
int get_opcode(char*, Functions[], int);
int get_funct(opcode, Functions[], int);
int get_the_func(char*, Functions[], int);
operands_num get_required_operands(opcode, Functions[], int);
int is_add_mode_valid(opcode, int, int, Functions[], int); /*Type means to source or destinion opernad.*/
int opcode_to_code(opcode);

/* -----------------------------------Data_struct---------------------------------------*/
struct Data
{
	unsigned int character : 7;
} typedef Data;

int is_data_exist(char*);

/* -----------------------------------Machine-code---------------------------------------*/

/*Data word helps us when we have for example call for a label,
It will be used twice - once for base addreass and twice for hist.*/
struct data_word
{
	unsigned int A_R_E : 3;
	unsigned int data : 16;
	unsigned exists : 2;
} typedef data_word;

/*Code word helps us to represent the first line of instruction function. */
struct code_word
{
	unsigned int A_R_E : 3;
	unsigned int funct : 4;
	unsigned int destination_register : 4;
	unsigned int destination_addressing : 2;
	unsigned int source_register : 4;
	unsigned int source_addressing : 2;
	unsigned exists : 2;
} typedef code_word;

/*Machine code should contain data word or code word (just one of them).*/
union word_machine_code
{
	data_word* dt;
	code_word* cd;
} typedef word_machine_code;

int IC;
int DC;
word_machine_code mc[8192];

enum addressing_modes
{
	imm_addressing = 0,
	direct_addressing = 1,
	index_addressing = 2,
	register_addressing = 3,
	invalid_addressing = 4
} typedef addressing_modes;

data_word* create_data_word(long, int, int);
code_word* create_code_word(Functions[], int, int, int, int, int, int, int, int);

/* -----------------------------------Registers---------------------------------------*/
enum registers
{
	r0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7,
	r8,
	r9,
	r10,
	r11,
	r12,
	r13,
	r14,
	r15
} typedef registers;

unsigned long convert_to_twos(signed long);
#endif