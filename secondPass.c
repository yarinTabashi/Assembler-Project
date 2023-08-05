#include "secondPass.h"

/*
* Adds the entry attributes to the symbols linkes list.
* @param head A pointer to the head of the symbols linked list.
* @param name The name of the label.
* @param line_num The line number in the file (in order to write on which line there is a bug. (if there is).
*/
int addAttributes(Symbol_node* head, char name[MAX_LINE_LENGTH], int line_num) {
	
	Symbol_node* current = head;
	int is_found = FALSE;
	
	while ((current != NULL) && (is_found==FALSE))
	{
		if (strcmp(current->name, name) == 0)
		{
			current->attributes[2] = 1;
			is_found = TRUE;
		}

		current = current->next;
	}
	if (is_found == FALSE)
	{
		printf("Error in line %d: %s\n", line_num, "The label hasn't found");
		return ERROR_VALUE;
	}

	return 1;
}