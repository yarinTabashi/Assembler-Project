#ifndef _SHARED_HEADER
#define _SHARED_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MACRO_NAME 31
#define MAX_LINES_MACRO 6
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define IC_INITALIZATION 100
#define DC_INITALIZATION 0

/* -----------------------------------Booleans ------------------------------------------------*/
enum bools
{
	FALSE = 0,
	TRUE = 1,
	ERROR_VALUE = -1
} typedef bools;

/*
* Allocate memory according to the required size. If it's failed - exit.
* @param size A size
* @return Returns a pointer in the requied size that allocated. If can't, it exit with error.
*/
void* safe_malloc(long size);
#endif
