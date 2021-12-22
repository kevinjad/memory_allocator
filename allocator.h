#include<unistd.h>
#include <stdio.h>

#define REQ_HEAP_SIZE 4096

typedef struct header{
	struct header* next;
	unsigned int size; // means.. size of header sized blocks
}Header;

Header base = {
	.next = &base,
	.size = 0
};

Header* free_chunks = &base;
Header* used_chunks;

void print_chunks(Header* chunks);

void add_to_free_chunks(Header* chunkp);

void* req_heap();

void* mem_alloc(size_t units);