#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mapalloc.h"

int main(void)
{
	const char buf[] = "THIS IS A CONSTANT STRING";

	char *ptr = MA_malloc(sizeof(buf));
	memcpy(ptr, buf, sizeof(buf));
	printf("%p: %s\n", ptr, ptr);

	MA_free(ptr);
	printf("freed\n");
	MA_free(ptr);
	printf("freed again\n");
}
