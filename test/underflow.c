#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include "mapalloc.h"

int main(void)
{
	char *ptr = MA_malloc(1);
	long pagesize = sysconf(_SC_PAGESIZE);
	printf("ptr: %p, pagesize %ld\n", ptr, pagesize);
	ptr[-1] = '\0';
	printf("shouldn't get here\n");
}
