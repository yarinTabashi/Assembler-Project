#include "text_ana.h"

/*The functions table (defined as static because it's not going to change).*/
static Functions functions_list[] =
{
	{"mov", mov_opcode, blank_funct, two_operands, mode_A, mode_B},
	{"cmp", cmp_opcode, blank_funct, two_operands, mode_A, mode_A},
	{"add", add_opcode, add_funct, two_operands, mode_A, mode_B},
	{"sub", sub_opcode, sub_funct, two_operands, mode_A, mode_B},
	{"lea", lea_opcode, blank_funct, two_operands, mode_C, mode_B},
	{"clr", clr_opcode, clr_funct, one_operand, mode_D, mode_B},
	{"not", not_opcode, not_funct, one_operand, mode_D, mode_B},
	{"inc", inc_opcode, inc_funct, one_operand, mode_D, mode_B},
	{"dec", dec_opcode, dec_funct, one_operand, mode_D, mode_B},
	{"jmp", jmp_opcode, jmp_funct, one_operand, mode_D, mode_C},
	{"bne", bne_opcode, bne_funct, one_operand, mode_D, mode_C},
	{"jsr", jsr_opcode, jsr_funct, one_operand, mode_D, mode_C},
	{"red", red_opcode, blank_funct, one_operand, mode_D, mode_B},
	{"prn", prn_opcode, blank_funct, one_operand, mode_D, mode_A},
	{"rts", rts_opcode, blank_funct, non_operands, mode_D, mode_D},
	{"stop", stop_opcode, blank_funct, non_operands, mode_D, mode_D}
};

/*
* Check if there is a label definition. If so, add it to the list.
* @param line Get a pointer to string (it's a specific line from the got file).
* @param line_number The line number in the file (in order to write on which line there is a bug. (if there is).
* @param is_definition
* @return Returns True(1) if it's a label. If not, False(0). If there is an issue returns Error-value.
*/
int is_label(char* line, int line_number, int is_definition, int which_pass)
{
	int counter = 0;
	int length = 0;

	char* pharse = skip_whitespaces(line);
	if (pharse[0] == '\0')
	{
		pharse++;
	}
	remove_whitespaces(pharse);
	length = word_length(pharse);

	if (is_definition)
	{
		if (pharse[length - 1] != ':')
		{
			return FALSE; /* In label definition, the last character must be a colon. */
		}
		pharse[length - 1] = '\0';
	}

	/* So it's a label definition. Let's check if it's set up properly. */
	if ((is_symbol_exist(symbols_list, pharse)) && (which_pass==1))
	{
		printf("Error in line %d: %s\n", line_number, symbol_exist_error);
		return ERROR_VALUE;
	}

	if (length > MAX_LABEL_LENGTH) /* Length's label must be a less than 31. */
	{
		printf("Error in line %d: %s\n", line_number, symbol_length_error);
		return ERROR_VALUE;
	}

	if (isalpha(pharse[0]) == 0) /* Check if the first chracter isn't a letter. */
	{
		printf("Error in line %d: %s\n", line_number, symbol_not_alpha_error);
		return ERROR_VALUE;
	}

	while (counter < length - 1) /* The characters must be letters or numbers. */
	{
		if ((isalpha(pharse[counter]) == 0) && (isdigit(pharse[counter]) == 0))
		{
			printf("Error in line %d: %s\n", line_number, symbol_alp_num_error);
			return ERROR_VALUE;
		}
		counter++;
	}

	/*Check if it is a reserved word.*/
	if ((is_function_exist(pharse, functions_list, sizeof(functions_list))) || (is_data_exist(pharse)))
	{
		printf("Error in line %d: %s\n", line_number, symbol_reserved_error);
		return ERROR_VALUE;
	}

	return TRUE;
}

/*
* Get a line and check if it's a storgae prompt.
* @param line Get a pointer to string (a line).
* @return Returns the data type Data/String/Entry/Extern. If it's not - returns Error-value.
*/
instructions_types get_data_type(char* line)
{
	char* pharse = get_first_word(line);

	if (pharse[0] == '\0')
	{
		pharse++;
		pharse = skip_whitespaces(pharse);
	}

	remove_whitespaces(pharse);

	if (strcmp(pharse, ".data") == 0)
	{
		return DATA_MARK;
	}

	if (strcmp(pharse, ".string") == 0)
	{
		return STRING_MARK;
	}

	if (strcmp(pharse, ".entry") == 0)
	{
		return ENTRY_MARK;
	}

	if (strcmp(pharse, ".extern") == 0)
	{
		return EXTERN_MARK;
	}

	return ERROR_VALUE; /*Returns an error if it is not one of the above options.*/
}

/*
* Get a pharse from file that should contains data. If exist, it insert it to the data storage.
* @param line Get a pointer to string (it's a pointer to after the call ".data").
* @param line_num The line number in the file (in order to write on which line there is a bug. (if there is).
* @return Returns counter of how much data added.
*/
int insert_data(char* line, int line_num)
{
	word_machine_code mac;
	double num = 0;
	int counter = 0;
	char* pharse = line;
	pharse = get_first_word(pharse);

	if (pharse[0] == '\0')
	{
		pharse++;
		pharse = skip_whitespaces(pharse);
	}
	remove_whitespaces(pharse);

	while (counter < MAX_LINE_LENGTH) /*The loop will be executed less than that. */
	{
		num = get_num(pharse, line_num);
		if (num == ERROR_VALUE)
		{
			return ERROR_VALUE;
		}

		/*Inserts the number to the data array.*/
		mac.dt = create_data_word((long)num, 0, 0);
		mc[IC] = mac;
		DC++;
		IC++;
		counter++;

		pharse = get_next_word(pharse);
		remove_whitespaces(pharse);

		/*Let's check if it's the last number.*/
		if (end_is_clear(pharse))
		{
			return counter;
		}
		else
		{
			/*After each parameter should be a comma. */
			if (pharse[0] != ',')
			{
				printf("Error in line %d: %s\n", line_num, "Missing comma");
				return ERROR_VALUE;
			}
			else
			{
				pharse++;
				pharse = get_next_word(pharse);
				if (pharse[0] == '\0')
				{
					pharse = get_next_word(pharse);
				}
				skip_whitespaces(pharse);
				remove_whitespaces(pharse);
			}
		}
	}
	return counter;
}

/*
* Get a pharse from file that should contains string. If exist, it insert it to the data storage.
* Every char translates to ascii and it's save it in the data storage. (In the end it save '\0' char).
* @param line Get a pointer to string (it's a pointer to after the call ".data").
* @param line_num The line number in the file (in order to write on which line there is a bug. (if there is).
* @return Returns counter of how much data added.
*/
int insert_string(char* line, int line_num)
{
	int length;
	int counter;
	char* pharse = line;
	word_machine_code mac;

	if (pharse[0] == '\0')
	{
		pharse++;
		pharse = skip_whitespaces(pharse);
	}
	remove_whitespaces(pharse);

	if (pharse[0] == '\0')
	{
		printf("Error in line %d: %s\n", line_num, "No data");
		return ERROR_VALUE;
	}

	length = word_length(pharse);
	counter = 1; /* Starting from 1 because it should skip the first quote. */

	if ((pharse[0] != '\"') || (pharse[length - 1] != '\"'))
	{
		printf("Error in line %d: %s\n", line_num, "Invalid string");
		return ERROR_VALUE;
	}

	/*Insert every character to the data array. */
	while (counter != length - 1)
	{
		mac.dt = create_data_word(pharse[counter], 0, 0);
		mc[IC] = mac;
		DC++;
		IC++;
		counter++;
	}

	/*Inserts the char EOF ('\0'), in order to say it's the end of this string. */
	mac.dt = create_data_word(0, 0, 0);
	mc[IC] = mac;
	DC++;
	IC++;

	return counter - 1; /*Becuase counter start from 1.*/
}

/*
* Creates an array that contains the data about the attributes.
* Details: [0]= Is data?        [1]= Is code?        [2]=Is entry?        [3]= Is exteranl?
*/
int* init_att(int is_data, int is_code, int is_entry, int is_external)
{
	int* arr = safe_malloc(sizeof(int) * 4);
	arr[0] = is_data;
	arr[1] = is_code;
	arr[2] = is_entry;
	arr[3] = is_external;
	return arr;
}

/*
* Gets a part from a line, and check what is the addressing mode.
* @param line Get a pointer to string (a part from a line).
* @param line_num The line number in the file (in order to write on which line there is a bug. (if there is).
* @param opcode The current opcode.
* @param is_first_operand TRUE(1) if it's the first operand and FALSE(0) if it's not.
* @param current_ic_index The current IC index
* @param whichPass 1 if it's the first pass, and 2 if it's the second pass.
* @param correctFileName The input file name.
* @return Returns the addressing mode.
*/
addressing_modes get_add_mode(char* line, int line_num, int opcode, int the_funct, int is_first_operand, int current_ic_index, int whichPass, char* correctFileName, FILE* ps_ext)
{
	int index = 0;
	int reg_num = 0;
	int num = 0;
	char* pharse = get_first_word(line);

	word_machine_code mac;

	if (end_is_clear(pharse))
	{
		printf("Error in line %d: %s\n", line_num, "The function should get operands but didn't get");
		return invalid_addressing;
	}

	if (pharse[0] == '#')
	{
		if (whichPass == 1)
		{
			mac.cd = create_code_word(functions_list, sizeof(functions_list), 4, the_funct, 0, imm_addressing, opcode, is_first_operand, current_ic_index);
			mc[current_ic_index] = mac;
		}

		if (is_first_operand)
		{
			IC++;
		}

		pharse++;
		if (whichPass == 1) {
			num = (long)get_num(pharse, line_num);
			if (num == ERROR_VALUE)
			{
				return ERROR_VALUE;
			}
			mac.dt = create_data_word(num, 0, 0);
			mc[IC] = mac;
			DC++;
		}
		IC++;
		return imm_addressing;
	}

	index = is_there_bracket(pharse); /*Get index of the opener bracket*/
	if ((whichPass == 2) && (index != ERROR_VALUE))
	{
		pharse[index] = '\0';
	}
	if ((index != ERROR_VALUE) && (whichPass==1))
	{
		pharse[index] = '[';
		pharse += index;

		pharse++;
		index = is_there_bracket(pharse); /*For the closer bracket*/
		if (index != ERROR_VALUE)
		{
			pharse[index] = '\0';
			pharse++;
			reg_num = get_num(pharse, line_num);

			if (reg_num == ERROR_VALUE)
			{
				return ERROR_VALUE;
			}

			if ((reg_num >= 10) && (reg_num <= 15))
			{
				if (whichPass == 1)
				{
					mac.cd = create_code_word(functions_list, sizeof(functions_list), 4, the_funct, reg_num, index_addressing, opcode, is_first_operand, current_ic_index);
					mc[IC] = mac;
				}

				if (is_first_operand)
				{
					IC++;
				}

				if (whichPass == 1) {
					IC += 2;
				}
				pharse[word_length(pharse)] = ']';
				return index_addressing;
			}
			else
			{
				printf("Error in line %d: %s\n", line_num, "Invalid register number");
				return invalid_addressing;
			}
		}
		else /*It means there are not brackerts*/
		{
			return invalid_addressing;
		}
	}
	else
	{
		if (is_symbol_exist(symbols_list, pharse))
		{
			if (whichPass == 1) 
			{
				mac.cd = create_code_word(functions_list, sizeof(functions_list), 4, the_funct, 0, direct_addressing, opcode, is_first_operand, current_ic_index);
				mc[current_ic_index] = mac;
			}
			if (is_first_operand)
			{
				IC++;
			}

			if (whichPass == 1) {
				IC += 2;

			}
			else if (whichPass == 2) {
				mac.dt = create_data_word((long)get_base_address(symbols_list, pharse), direct_addressing, is_extern(symbols_list, pharse));
				mc[IC] = mac;
				IC++;
				mac.dt = create_data_word(get_offset(symbols_list, pharse), direct_addressing, is_extern(symbols_list, pharse));
				mc[IC] = mac;
				IC++;
				ps_ext = printExtern(symbols_list, pharse, ps_ext, IC, correctFileName);
			}

			return direct_addressing;
		}
	}
	if (pharse[0] != 'r')
	{
		if (whichPass == 1) 
		{
			mac.cd = create_code_word(functions_list, sizeof(functions_list), 4, the_funct, 0, direct_addressing, opcode, is_first_operand, current_ic_index);
			mc[current_ic_index] = mac;
		}
		if (is_first_operand)
		{
			IC++;
		}

		/*Canno't fill it in this pass. So now we just prepare the storage.*/
		if (whichPass == 1) {
			IC += 2;
		}
		else if (whichPass == 2) {
			mac.dt = create_data_word((long)get_base_address(symbols_list, pharse), direct_addressing, is_extern(symbols_list, pharse));
			mc[IC] = mac;
			IC++;
			mac.dt = create_data_word(get_offset(symbols_list, pharse), direct_addressing, is_extern(symbols_list, pharse));
			mc[IC] = mac;
			IC++;
			ps_ext = printExtern(symbols_list, pharse, ps_ext, IC, correctFileName);
		}

		return direct_addressing;
	}
	pharse++;
	reg_num = get_num(pharse, line_num);
	if (reg_num == ERROR_VALUE)
	{
		return invalid_addressing;
	}
	if ((reg_num >= 0) && (reg_num <= 15))
	{
		if (whichPass == 1)
		{
			mac.cd = create_code_word(functions_list, sizeof(functions_list), 4, the_funct, reg_num, register_addressing, opcode, is_first_operand, current_ic_index);
			mc[current_ic_index] = mac;
		}
		
		if (is_first_operand)
		{
			IC++;
		}

		return register_addressing;
	}
	else
	{
		printf("Error in line %d: %s\n", line_num, "Invalid register number");
		return invalid_addressing;
	}
}

/*
* Gets a part from a line, and do the processing to figure out the operands.
* @param line Get a pointer to string (a part from a line).
* @param line_num The line number in the file (in order to write on which line there is a bug. (if there is).
* @param whichPass 1 if it's the first pass, and 2 if it's the second pass.
* @param correctFileName The input file name.
* @return Returns True(1) if allright is ok. If there is a bug, returns Error-Value.
*/
int process_operands(char* line, opcode opcode, funct the_funct, int line_num, int whichPass, char* correctFileName, FILE* ps_ext)
{
	char* pharse;
	operands_num req_num_operands = 0;
	addressing_modes add_mode = 0;
	int had_comma = FALSE;
	int comma_index = 0;
	int current_ic_index = IC;

	req_num_operands = get_required_operands(opcode, functions_list, sizeof(functions_list));

	/*If the function shouldn't get operands. */
	if (req_num_operands == non_operands)
	{
		if (end_is_clear(line))
		{
			return TRUE;
		}
		else
		{
			printf("Error in line %d: %s\n", line_num, "Extra text: the function shouldn't get operands");
			return ERROR_VALUE;
		}
	}
	pharse = get_first_word(line);

	if (pharse[comma_index = word_length(pharse)] == ',')
	{
		pharse[word_length(pharse)] = '\0';
		had_comma = TRUE;
	}

	/*If the function should get operands*/
	if ((req_num_operands == one_operand) || (req_num_operands == two_operands))
	{
		add_mode = get_add_mode(pharse, line_num, opcode, the_funct, TRUE, current_ic_index, whichPass, correctFileName, ps_ext);
		if (add_mode == invalid_addressing)
		{
			return ERROR_VALUE;
		}
		if (!is_add_mode_valid(opcode, add_mode, SOURCE_OPERAND, functions_list, sizeof(functions_list)))
		{
			printf("Error in line %d: %s\n", line_num, "Invalid addressing mode");
			return ERROR_VALUE;
		}

		if (had_comma)
		{
			pharse[comma_index] = ',';
			pharse += comma_index;
			pharse = get_first_word(pharse);
		}
		else
		{
			pharse = get_next_word(pharse);
		}

		if (req_num_operands == one_operand)
		{
			if ((!end_is_clear(pharse)) && (whichPass ==1))
			{
				printf("Error in line %d: %s\n", line_num, "Extra text: This function should get one operand, but there is extra text after it");
				return ERROR_VALUE;
			}
			return TRUE;
		}

	/* Process in order to find the second opernad: */
		if ((pharse[0] != ',') && (had_comma == FALSE))
		{
			if (pharse[0] == '\0')
			{
				printf("Error in line %d: %s\n", line_num, "Missing destination operand");
				return ERROR_VALUE;
			}
			if (whichPass == 1)
			{
				printf("Error in line %d: %s\n", line_num, "Missing comma after the first operand");
				return ERROR_VALUE;
			}
		}
		else if (pharse[0] == ',')
		{
			pharse++;
			pharse = get_first_word(pharse);
		}
		add_mode = get_add_mode(pharse, line_num, opcode, the_funct, FALSE, current_ic_index, whichPass, correctFileName, ps_ext);
		if (add_mode == invalid_addressing)
		{
			return ERROR_VALUE;
		}
		if (!is_add_mode_valid(opcode, add_mode, DESTINATION_OPERAND, functions_list, sizeof(functions_list)))
		{
			printf("Error in line %d: %s\n", line_num, "Invalid addressing mode (at destination)");
			return ERROR_VALUE;
		}
		pharse = get_next_word(pharse);

		if ((!end_is_clear(pharse)) && (whichPass==1))
		{
			printf("Error in line %d: %s\n", line_num, "Extra text: This function should get two operands, but there is extra text after it");
			return ERROR_VALUE;
		}
		return TRUE;
	}
	return TRUE;
}

/*
* This function do the macros' spread process.
* @param file - pointer to the source file.
* @param new_file - pointer to the new file that will save the this process.
*/
int handle_macros(FILE* file, FILE* new_file)
{
	char* cur_line = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
	char* pharse = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
	char* macro_content = (char*)malloc(MAX_LINE_LENGTH * MAX_LINES_MACRO * sizeof(char));
	int has_macro_flag = FALSE; /*Flags if it's a macro defenition. */
	int flag_copy = TRUE;
	char* original_line = (char*)malloc((strlen(cur_line) + 1) *  sizeof(char));

	Macro_node* current_macro = (Macro_node*)malloc(sizeof(Macro_node));
	current_macro->name = (char*)malloc(MAX_MACRO_NAME * sizeof(char));
	current_macro->content = (char*)malloc(MAX_LINE_LENGTH * MAX_LINES_MACRO * sizeof(char));

	if (file == NULL)
	{
		printf("OOPS! File didn't found! ");
		return ERROR_VALUE;
	}

	while (fgets(cur_line, MAX_LINE_LENGTH, file))
	{
		flag_copy = TRUE;
		strcpy(original_line, cur_line);
		pharse = get_first_word(cur_line);

		if (has_macro_flag == TRUE)
		{
			if (strcmp(pharse, "endm") == 0)
			{
				if (macros_list == NULL)
				{
					macros_list = current_macro;
				}
				else
				{
					add_macro(macros_list, current_macro->name, current_macro->content);
				}
				has_macro_flag = FALSE;
			}
			else
			{
				current_macro->content = strallocat(current_macro->content, original_line);
			}
		}
		else
		{
			if (macros_list != NULL)
			{
				macro_content = search_macro(macros_list, pharse);
				if (macro_content != NULL) /*If this line is call for macro has defined. */
				{
					fputs(macro_content, new_file);
					flag_copy = FALSE;
				}
			}

			if (strcmp(pharse, "macro") == 0) /*If this line is definning of new macro*/
			{
				strcpy(current_macro->name, get_next_word(pharse));
				/* Remove two trailing whitespaces */
				current_macro->name[strlen(current_macro->name) - 1] = '\0';
				current_macro->name[strlen(current_macro->name) - 1] = '\0';
				strcpy(current_macro->content, "");
				current_macro->next = NULL;
				has_macro_flag = TRUE;
				flag_copy = FALSE;
			}

			if (flag_copy == TRUE) /*It means this line should write in the new file.*/
			{
				fputs(original_line, new_file);
			}
		}
	}

	return TRUE;
}


int compile_single_file(char* current_arg) {
	/*Creates pointers to the files: source file, file after macros spreads and output files. */
	word_machine_code mac; /*A temporary variable that saves the word that is created.*/
	FILE* f_source;
	FILE* f_after_pre;
	FILE* ps_ent = NULL;
	FILE* ps_ext = NULL;
	char* cur_line = (char*)safe_malloc(MAX_LINE_LENGTH * sizeof(char));
	char* label_name_pass1 = (char*)safe_malloc(MAX_LINE_LENGTH * sizeof(char));
	char* label_name_pass2 = (char*)safe_malloc(MAX_LINE_LENGTH * sizeof(char));
	int were_error = FALSE;
	int is_symbol_define = FALSE;
	int should_create_ext = FALSE;
	instructions_types data_type = 0;
	int lines_counter = 1;
	int opcode = 0;
	char fullFileName[100] = { '\0' };
	char correctFileName[100] = { '\0' };
	int currentIndexExt;
	char extFileName[104] = { '\0' };
	char newFileName[103];
	int currentIndex;
	int the_funct = 0;
	int temp = 0;

	IC = IC_INITALIZATION; 
	DC = DC_INITALIZATION;
	symbols_list = NULL;
	macros_list = NULL;

	strcpy(fullFileName, current_arg);

	/*Creates the outputs files names. */
	currentIndex = howLong(fullFileName);
	strcpy(correctFileName, fullFileName);
	if (strlen(correctFileName) < 3 || correctFileName[currentIndex - 3] != '.' || correctFileName[currentIndex - 2] != 'a' || correctFileName[currentIndex - 1] != 's') {
		printf("Error: Parameter %s is not an assembly file \n", correctFileName);
		return FALSE;
	}

	printf("Starting compliation of file \"%s\" \n", fullFileName);

	correctFileName[currentIndex - 1] = '\0';
	correctFileName[currentIndex - 2] = '\0';
	correctFileName[currentIndex - 3] = '\0';

	currentIndex = howLong(correctFileName);
	strcpy(newFileName, correctFileName);
	newFileName[currentIndex] = '.';
	newFileName[currentIndex + 1] = 'a';
	newFileName[currentIndex + 2] = 'm';
	newFileName[currentIndex + 3] = '\0';
	f_source = fopen(fullFileName, "r");
	if (f_source == NULL) {
		printf("Failed to open file \"%s\"\n", fullFileName);
		return FALSE;
	}

	f_after_pre = fopen(newFileName, "w");

	/*Pre-processor: macro's spread */
	if (handle_macros(f_source, f_after_pre) == ERROR_VALUE)
	{
		fclose(f_source);
		fclose(f_after_pre);
		exit(0); /*If were errors. Anyway, assume there are no errors at this time. */
	}

	fclose(f_after_pre);
	fclose(f_source);

	f_after_pre = fopen(newFileName, "r"); /*This file will be opened later, but for reading only.*/
	while (fgets(cur_line, MAX_LINE_LENGTH, f_after_pre)) {
		is_symbol_define = FALSE;
		cur_line = skip_whitespaces(cur_line);

		if ((cur_line[0] == '\0') || (cur_line[0] == ';')) /* Check if it's a blank line. */
		{
			lines_counter++;
			continue;
		}

		if (is_label(cur_line, lines_counter, TRUE, 1))
		{
			memcpy(label_name_pass1, cur_line, strlen(cur_line) + 1);
			cur_line = get_next_word(cur_line);
			cur_line++;
			cur_line = get_first_word(cur_line);
			is_symbol_define = TRUE;
		}

		data_type = get_data_type(cur_line);

		if ((data_type == DATA_MARK) || (data_type == STRING_MARK))
		{
			cur_line = get_next_word(cur_line);
			if (is_symbol_define)
			{
				add_symbol(&symbols_list, label_name_pass1, IC, init_att(TRUE, FALSE, FALSE, FALSE));
			}

			if (data_type == DATA_MARK)
			{
				temp = insert_data(cur_line, lines_counter);
			}
			else
			{
				temp = insert_string(cur_line, lines_counter);
			}

			if ((were_error == FALSE) && (temp == ERROR_VALUE))
			{
				were_error = TRUE; /*Although there was an error - it continue the pass.*/
			}
		}
		else if ((data_type == ENTRY_MARK) || (data_type == EXTERN_MARK))
		{
			if (data_type == ENTRY_MARK)
			{
				lines_counter++;
				continue;
			}
			else if (data_type == EXTERN_MARK)
			{
				cur_line = get_next_word(cur_line);
				remove_whitespaces(cur_line);
				memcpy(label_name_pass1, cur_line, strlen(cur_line) + 1);

				if (is_label(label_name_pass1, lines_counter, FALSE, 2) == FALSE)
				{
					were_error = TRUE;
				}
				else
				{
					add_symbol(&symbols_list, label_name_pass1, 0, init_att(FALSE, FALSE, FALSE, TRUE));
					should_create_ext = TRUE;
				}
			}
		}
		else
		{
			/*So it's instruction line */
			if (is_symbol_define)
			{
				add_symbol(&symbols_list, label_name_pass1, IC, init_att(FALSE, TRUE, FALSE, FALSE));

			}

			if (is_function_exist(cur_line = get_first_word(cur_line), functions_list, sizeof(functions_list)))
			{
				opcode = get_opcode(get_first_word(cur_line), functions_list, sizeof(functions_list));
				the_funct = get_the_func(get_first_word(cur_line), functions_list, sizeof(functions_list));
				mac.dt = create_data_word(opcode_to_code(opcode), 0, 0);
				mc[IC] = mac;
				DC++;
				IC++;
				temp = process_operands(get_next_word(cur_line), opcode, the_funct, lines_counter, 1, correctFileName, ps_ext);
				if (temp == ERROR_VALUE)
				{
					were_error = TRUE;
				}
			}
			else
			{
				printf("Error in line %d: %s\n", lines_counter, "Instruction (function) name is not valid");
				were_error = TRUE;
			}

		}
		lines_counter++;
	}

	fclose(f_after_pre);
	/*If were errors, it should stop now. */
	if (were_error)
	{
		return FALSE;
	}

	if (should_create_ext) {
		currentIndexExt = howLong(correctFileName);
		strcpy(extFileName, correctFileName);
		extFileName[currentIndexExt] = '.';
		extFileName[currentIndexExt + 1] = 'e';
		extFileName[currentIndexExt + 2] = 'x';
		extFileName[currentIndexExt + 3] = 't';
		extFileName[currentIndexExt + 4] = '\0';
		ps_ext = fopen(extFileName, "w");
	}

	/*The end of pass 1, The beginning of pass 2.*/
	f_after_pre = fopen(newFileName, "r");

	/*Initialize the counters. */
	DC = DC_INITALIZATION;
	IC = IC_INITALIZATION;
	lines_counter = 1;

	while (fgets(cur_line, MAX_LINE_LENGTH, f_after_pre)) {
		cur_line = skip_whitespaces(cur_line);

		if (is_label(cur_line, lines_counter, TRUE, 2))
		{
			cur_line = get_next_word(cur_line);
			cur_line++;
			cur_line = get_first_word(cur_line);
		}


		if (cur_line[0] == '\0') {
			continue;
		}

		data_type = get_data_type(cur_line);
		if (data_type == DATA_MARK || data_type == STRING_MARK || data_type == EXTERN_MARK) {
			cur_line = get_next_word(cur_line);
			if (data_type == DATA_MARK)
			{
				temp = insert_data(cur_line, lines_counter);
			}
			else if (data_type == STRING_MARK)
			{
				temp = insert_string(cur_line, lines_counter);
			}

			if ((were_error == FALSE) && (temp == ERROR_VALUE))
			{
				were_error = TRUE;
			}
			continue;
		}

		if (data_type == ENTRY_MARK) {
			cur_line = get_next_word(cur_line);
			remove_whitespaces(cur_line);
			memcpy(label_name_pass2, cur_line, strlen(cur_line) + 1);

			if (is_label(label_name_pass2, lines_counter, FALSE, 2) == FALSE)
			{
				were_error = TRUE;
			}
			else {
				cur_line = get_first_word(cur_line);
				if (addAttributes(symbols_list, cur_line, lines_counter) == ERROR_VALUE) {
					were_error = TRUE;
				}
			}
		}
		else {
			if (is_function_exist(cur_line = get_first_word(cur_line), functions_list, sizeof(functions_list)))
			{
				IC++;
				opcode = get_opcode(get_first_word(cur_line), functions_list, sizeof(functions_list));
				the_funct = get_the_func(get_first_word(cur_line), functions_list, sizeof(functions_list));
				process_operands(get_next_word(cur_line), opcode, the_funct, lines_counter, 2, correctFileName, ps_ext);
			}
		}
		lines_counter++;
	}

	/*If were errors, it should stop now. */
	if (were_error)
	{
		return FALSE;
	}

	printEntry(symbols_list, ps_ent, correctFileName);

	/*Closing the output files. */
	if (ps_ent != NULL) {
		fclose(ps_ent);
	}

	if (ps_ext != NULL) {
		fclose(ps_ext);
	}

	printFiles(correctFileName, IC);

	printf("Complication of file \"%s\" finished successfully\n", fullFileName);

	return TRUE;
}

int main(int argc, char* argv[])
{
	int i;
	int error_in_any_file = FALSE;

	if (argc <= 1) {
		printf("Error: The program must get an assembly file as an input\n");
		return FALSE;
	}

	/*Loop - If there are more than one file.*/
	for(i = 1; i < argc; i++){
		if (!compile_single_file(argv[i])) {
			error_in_any_file = TRUE;
		}
	}

	return error_in_any_file;
}