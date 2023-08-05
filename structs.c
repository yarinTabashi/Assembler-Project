#include "structs.h"
#include <math.h>

/*
* Adds a macro node to the end of the got macro list.
* @param head A pointer to the head of the macro's linked list.
* @param name The name of the new macro.
* @param content The content of the new macro (from "macro" to "endm").
* @return A pointer to the added node.
*/
Macro_node* add_macro(Macro_node* head, char* name, char* content)
{
	Macro_node* current = head;
	Macro_node* new_macro = (Macro_node*)safe_malloc(sizeof(Macro_node));
	new_macro->name = safe_malloc(sizeof(char) * MAX_LABEL_LENGTH);
	strcpy(new_macro->name, name);
	new_macro->content = safe_malloc(sizeof(char) * MAX_LINE_LENGTH * 6);
	strcpy(new_macro->content, content);

	new_macro->next = (Macro_node*)safe_malloc(sizeof(Macro_node));
	new_macro->next = NULL;

	if (head == NULL)
	{
		return new_macro;
	}

	while (current->next != NULL)
	{
		current = current->next;
	}

	current->next = new_macro;
	return new_macro;
}

/*
* Searches a macro by name. If exist, returns this node.
* @param head A pointer to the head of the macro's linked list.
* @param name The name of the macro.
* @param content The content of the new macro (from "macro" to "endm").
* @return A pointer to the requested macro node.
*/
char* search_macro(Macro_node* head, char* name)
{
	Macro_node* current = head;

	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			return current->content;
		}
		current = current->next;
	}

	if (current == NULL)
	{
		return NULL;
	}
	return current->content;
}

/*
* Calculates the base address by operand value.
* @param operand_value
* @return Returns the calculated base address.
*/
int calc_base_address(int operand_value)
{
	if (operand_value <= 1)
	{
		return 0;
	}

	operand_value--;
	while (operand_value != 0)
	{
		if (operand_value % 16 == 0)
		{
			return operand_value;
		}
		operand_value--;
	}

	return 0;
}

/*
* Calculates the hist by operand value.
* @param operand_value
* @return Returns the calculated hist.
*/
int calc_offset(int operand_value)
{
	if (operand_value <= 0)
	{
		return 0;
	}
	return operand_value - calc_base_address(operand_value);
}

/*
* Searches specific symbol in the symbols list by name, and returns the base address.
* @param head A pointer to the head of the symbols linked list.
* @param name A name of a symbol.
* @return Returns the base address of the specific symbol. If not exist, returns 0.
*/
int get_base_address(Symbol_node* head, char* name)
{
	Symbol_node* current = head;

	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			return current->base_address;
		}
		current = current->next;
	}

	return 0;
}

/*
* Searches specific symbol in the symbols list by name, and returns the hist.
* @param head A pointer to the head of the symbols linked list.
* @param name A name of a symbol.
* @return Returns the hist of the specific symbol. If not exist, returns 0.
*/
int get_offset(Symbol_node* head, char* name)
{
	Symbol_node* current = head;

	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			return current->offset;
		}
		current = current->next;
	}

	return 0;
}

/*
* Searches specific symbol in the symbols list by name, and returns the attributes.
* @param head A pointer to the head of the symbols linked list.
* @param name A name of a symbol.
* @return Returns pointer to int array, size of 4 cells. Every cell equals to False(0) or True(1).
* Details: [0]= Is data?	[1]= Is code?	[2]=Is entry?	[3]= Is exteranl?
*/
int* get_attributes(Symbol_node* head, char* name)
{
	Symbol_node* current = head;

	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			return current->attributes;
		}
		current = current->next;
	}

	return 0;
}

/*
* Checks if specific symbol is extern symbol.
* @param head A pointer to the head of the symbols linked list.
* @param name A name of a symbol.
* @return Returns True(1) if it's extern symbol. Else, return False(0).
* Details: [0]= Is data?	[1]= Is code?	[2]=Is entry?	[3]= Is exteranl?
*/
int is_extern(Symbol_node* head, char* name)
{
	Symbol_node* current = head;

	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			return current->attributes[3];
		}
		current = current->next;
	}

	return -1;
}

/*
* Adds a symbol node to the end of the got symbols list.
* @param head A pointer to pointer of head symbols linked list.
* @param name The name of the new symbol.
* @param value The value of the symbol.
* @param attributes An array the stores the symbol's properties.
* @return A pointer to the added node.
*/
Symbol_node* add_symbol(Symbol_node** head, char* name, int value, int attributes[])
{
	Symbol_node* new_symbol = safe_malloc(sizeof(Symbol_node));
	Symbol_node* last = *head;
	new_symbol->value = value;
	new_symbol->base_address = calc_base_address(value);
	new_symbol->offset = calc_offset(value);
	new_symbol->name = safe_malloc(sizeof(char) * MAX_LABEL_LENGTH);
	new_symbol->attributes[0] = attributes[0];
	new_symbol->attributes[1] = attributes[1];
	new_symbol->attributes[2] = attributes[2];
	new_symbol->attributes[3] = attributes[3];

	strcpy(new_symbol->name, name);
	new_symbol->next = NULL;

	if (*head == NULL)
	{
		*head = new_symbol;
	}
	else
	{
		while (last->next != NULL)
		{
			last = last->next;
		}
		last->next = new_symbol;
	}

	return *head;
}

/*
* Searches a symbol by name, and checks if it exist.
* @param head A pointer to pointer of head symbols linked list.
* @param name A name of a symbol to search.
* @return Returns True if there is a symbol with the got name. Else, returns False.
*/
int is_symbol_exist(Symbol_node* head, char* name)
{
	Symbol_node* current = head;
	while (current != NULL)
	{
		if (strcmp(current->name, name) == 0)
		{
			return TRUE;
		}

		current = current->next;
	}

	return FALSE;
}
/*
* Checks if there is a function with the got name.
* @param name A name of a function to search.
* @return Returns True(1) if found a function with this name. Else, returns False(0).
*/
int is_function_exist(char* name, Functions functions_list[], int functCount)
{
	int i;

	for (i = 0; i < functCount / sizeof(functions_list[0]); i++)
	{
		if (strcmp(functions_list[i].name, name) == 0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

int is_data_exist(char* name)
{
	return FALSE;
}

data_word* create_data_word(long the_data, int addressing_mode, int is_extern)
{
	signed long x;
	unsigned long mask;
	unsigned long ARE;
	unsigned long data;
	data_word* dt = (data_word*)safe_malloc(sizeof(data_word));
	if (dt == NULL)
	{
		exit(1);
	}

	x = -1; /*We use two's complement. So x= 1111.....1111 (total - 32 bits of 1) */
	mask = x; /*So mask=1111....1111 (total - 32 bits of 1) */
	ARE = 4; /*It means A=1 and the rest zeros (0100) */
	data = convert_to_twos(the_data);
	mask = mask >> 13; /*mask= 0000....0(13)...1111 ..... 1(19) */
	dt->data = mask & data;
	if ((addressing_mode == direct_addressing) || (addressing_mode == index_addressing))
	{
		if (is_extern)
		{
			dt->A_R_E = 1; /**/
		}
		else
		{
			dt->A_R_E = 2;
		}
	}
	else
	{
		dt->A_R_E = ARE; /*A=1,R=0,E=0*/
	}

	dt->exists = 1;

	return dt;
}

code_word* create_code_word(Functions functions_list[], int functCount, int are, int funct, int reg, int add, int opcode, int is_first_operand, int ic_index)
{
	int required_operands = get_required_operands(opcode, functions_list, functCount);
	code_word* cd = (code_word*)safe_malloc(sizeof(code_word));
	cd->A_R_E = are;
	cd->funct = funct;

	if ((is_first_operand == TRUE) && (required_operands == two_operands))
	{
		cd->source_addressing = add;
		cd->source_register = reg;
		cd->destination_addressing = 0;
		cd->destination_register = 0;
	}
	else
	{
		if ((is_first_operand == FALSE) && (required_operands == two_operands))
		{
			cd->source_addressing = mc[ic_index].cd->source_addressing;
			cd->source_register = mc[ic_index].cd->source_register;
		}
		else
		{
			cd->source_addressing = 0;
			cd->source_register = 0;
		}
		cd->destination_addressing = add;
		cd->destination_register = reg;
	}

	cd->exists = 2;


	return cd;
}

/*
* Searches function by name and returns their opcode.
* @param function_name A fucntion to search in the functions list.
* @return Returns the opcode of the requested function.
*/
int get_opcode(char* function_name, Functions functions_list[], int functCount)
{
	int i;

	for (i = 0; i < functCount / sizeof(functions_list[0]); i++)
	{
		if (strcmp(functions_list[i].name, function_name) == 0)
		{
			return functions_list[i].opcode_num;
		}
	}

	return 0;
}

int get_funct(opcode opcode, Functions functions_list[], int functCount)
{
	int i;

	for (i = 0; i < functCount / sizeof(functions_list[0]); i++)
	{
		if (functions_list[i].opcode_num == opcode)
		{
			return functions_list[i].funct_num;
		}
	}

	return 0;
}

int get_the_func(char* function_name, Functions functions_list[], int functCount)
{
	int i;

	for (i = 0; i < functCount / sizeof(functions_list[0]); i++)
	{
		if (strcmp(functions_list[i].name, function_name) == 0)
		{
			return functions_list[i].funct_num;
		}
	}

	return 0;
}

/*
* Returns the required number of operands, according to opcode.
* @param opcode A opcode number of function.
* @return The required number of operands.
*/
operands_num get_required_operands(opcode opcode, Functions functions_list[], int functCount)
{
	int i;

	for (i = 0; i < functCount / sizeof(functions_list[0]); i++)
	{
		if (functions_list[i].opcode_num == opcode)
		{
			return functions_list[i].req_operands_num;
		}
	}

	return 0;
}

/*
* Check if function can be used with specific address mode (for source and destination).
* @param opcode Get an opcode in order to search the request function.
* @param add_mode
* @param type_operand It means source (if it's the first) or destination opearnd (if it's the second).
* @return Returns True(1) if it function can be used with this address mode. Else, False(0).
*/
int is_add_mode_valid(opcode opcode, int add_mode, int type_operand, Functions functions_list[], int functCount)
{
	/* All functions that should get one operand - should get destination operand, and not source operand.*/
	if (get_required_operands(opcode, functions_list, functCount) == one_operand)
	{
		type_operand = DESTINATION_OPERAND;
	}

	if (type_operand == SOURCE_OPERAND)
	{
		return functions_list[opcode].valid_source_modes[add_mode];
	}

	if (type_operand == DESTINATION_OPERAND)
	{
		return functions_list[opcode].valid_dest_modes[add_mode];
	}

	return ERROR_VALUE;
}

/*
* Get a number and convert it to be represented by the Two's complement.
* @param num A negative or positive number.
* @return An unsigned long, that represents the got number in Two's complement.
*/
unsigned long convert_to_twos(signed long num)
{
	unsigned long new_num;

	if (num > 0)
	{
		return num;
	}

	new_num = abs(num);
	new_num = ~new_num;
	new_num += 1;
	return new_num;
}

int opcode_to_code(opcode num)
{
	return pow(2, num);
}