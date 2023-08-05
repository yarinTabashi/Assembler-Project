#include "shared.h"

void* safe_malloc(long size)
{
	void* the_pointer = malloc(size);
	if (the_pointer == NULL)
	{
		printf("OOPS! Couldn't do memory allocation.");
		exit(1);
	}
	return the_pointer;
}