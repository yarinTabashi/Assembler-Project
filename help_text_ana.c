#include "help_text_ana.h"

/*
* Skips the whitespaces that appears on the beggining of the pharse (if exist).
* @param pharse A string (pointer to char).
* @return Return pointer to first char after the whitespaces.
*/
char* skip_whitespaces(char* pharse)
{
    int i = 0;
    if (pharse[0] == '\"')
    {
        return pharse;
    }

    for (i = 0; (pharse[i] != 0) && (isspace(pharse[i]) != 0); i++); /* Check how much whitespaces there are. */

    return pharse + i;
}

/*
* Remove the whitespaces that appears on the end of the pharse (if exist).
* @param pharse A string (pointer to char).
*/
void remove_whitespaces(char* pharse)
{
    int i = 0;

    for (i = 0; (pharse[i] != '\0') && (isspace(pharse[i]) == 0); i++);

    pharse[i] = '\0';
}

/*
* Returns the length of the pharse.
* @param pharse A string (pointer to char).
* @return Word length of the first pharse (until the first whitespace).
*/
int word_length(char* pharse)
{
    int counter = 0;

    while ((pharse[counter] != '\0') && (isspace(pharse[counter]) == 0) && (pharse[counter] != ','))
    {
        counter++;
    }

    return counter;
}

/*
* Returns the first pharse that appears.
* @param pharse A string (pointer to char).
* @return A pointer to the first pharse.
*/
char* get_first_word(char* pharse)
{
    char* new_pharse;
    if (pharse[0] == '\0')
    {
        pharse++;
    }

    new_pharse = skip_whitespaces(pharse);
    remove_whitespaces(new_pharse);
    return new_pharse;
}

/*
* Skips the first pharse.
* @param pharse A string (pointer to char).
* @return A pointer to second pharse (after the first whitespace).
*/
char* get_next_word(char* pharse)
{
    pharse += word_length(pharse);
    if (pharse[0] == '\0')
    {
        pharse++;
    }

    pharse = skip_whitespaces(pharse);

    return pharse;
}

/*
* Check if there are bracket square (opener or closer). If so, returns their index.
* @param pharse A string (pointer to char).
* @return The index of the first bracket square that appears. If not, returns error value.
*/
int is_there_bracket(char* pharse)
{
    int i = 0;
    for (i = 0; (pharse[i] != '\0') && (isspace(pharse[i]) == 0) && (pharse[i] != '[') && (pharse[i] != ']'); i++);

    if ((pharse[i] != '[') && (pharse[i] != ']'))
    {
        return -1;
    }

    return i;
}

bools is_integer(double num)
{
    int convertedNum = (int)num;
    return (num == convertedNum);
}

/*
* Returns the number that found at the beggining of the pharse (if there is).
* @param pharse A string (pointer to char).
* @return The number that found. If not, returns error value.
*/
int get_num(char* pharse, int line_num)
{
    int num;
    char* ptr;
    num = (int)strtod(pharse, &ptr); /* This function returns 0 if doesn't exist a number. */

    if (num == 0)
    {
        if (pharse[0] != '\0') /* Check if it's not the end of the string. */
        {
		    printf("Error in line %d: %s\n", line_num, "Invalid parameter - it's not a number");
            return ERROR_VALUE;
        }
		printf("Error in line %d: %s\n", line_num, "Missing number");
        return ERROR_VALUE;
    }

    if (is_integer(num) == FALSE)
    {
		printf("Error in line %d: %s\n", line_num, " The number must be an integer");
        return ERROR_VALUE;
    }

    return num;
}

/*
* Check if this is the end of the line.
* @param pharse A string (pointer to char).
* @return True if it is the end. False, if not.
*/
int end_is_clear(char* line)
{
    while (line != NULL)
    {
        if (line[0] == '\0') /* This char symbolize end of the line. */
        {
            return 1;
        }

        if (isspace(line[0]) == 0) /* If it's not a whitespace */
        {
            return 0;
        }
        line++;
    }

    return 1;
}

char* strallocat(char* s0, char* s1)
{
    char* str = (char*)safe_malloc((strlen(s0) + strlen(s1) + 1) * sizeof(char));
    strcpy(str, s0);
    strcat(str, s1);
    return str;
}
