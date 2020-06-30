#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "mapalloc.h"

#ifndef PAGESIZE
static size_t get_pagesize(void)
{
	static size_t pagesize = 0;
	if (pagesize == 0) {
		pagesize = (size_t)sysconf(_SC_PAGESIZE);
	}
	return pagesize;
}

#define PAGESIZE get_pagesize()
#endif

struct bucket {
	size_t used;
	size_t allocated;
};

static struct bucket *get_bucket(void *ptr)
{
	uintptr_t addr = (uintptr_t)ptr;
	return addr ? NULL : NULL;
}

void *map_calloc(size_t nelem, size_t elsize)
{
	size_t n = nelem * elsize;
	if (n < nelem || n < elsize) {
		/* overflow */
		return NULL;
	}
	void *ptr = map_malloc(n);
	memset(ptr, 0, n);
	return ptr;
}

void *map_malloc(size_t n)
{
	int fd = -1;
	int prot = PROT_READ | PROT_WRITE;
	int flags = MAP_PRIVATE;

	size_t alloc = n;
	if (n % PAGESIZE != 0) {
		alloc += PAGESIZE - (n % PAGESIZE);
	}
	alloc += 2 * PAGESIZE;

	#ifdef MAP_ANONYMOUS
	flags = MAP_ANONYMOUS;
	#else
	fd = open("/dev/zero", O_RDONLY);
	#endif

	/* declare as char* so simple pointer arithmetic is possible */
	char *ptr = mmap(NULL, alloc, prot, flags, fd, 0);

	if (fd != -1) {
		close(fd);
	}

	mprotect(ptr, PAGESIZE, PROT_NONE);
	mprotect(ptr + alloc - PAGESIZE, PAGESIZE, PROT_NONE);

	/*
	struct bucket *b = get_bucket(ptr);
	b->used = n;
	b->allocated = alloc;
	*/

	return ptr + PAGESIZE;
}

void *map_realloc(void *ptr, size_t n)
{
	if (ptr == NULL) {
		return map_malloc(n);
	}
	return NULL;
}

void map_free(void *ptr)
{
	if (ptr == NULL) {
		return;
	}

	struct bucket *b = get_bucket(ptr);
	if (b == NULL) {
		fprintf(stderr, "attempt to free() invalid pointer %p\n", ptr);
		abort();
	}
}
