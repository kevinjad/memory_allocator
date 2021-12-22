#include "allocator.h"

void print_chunks(Header* chunks){
	Header* p = chunks;
	Header* prev = NULL;
	int i =0;
	for(p = chunks; p>prev ;p=p->next, i++){
		printf("Chunk#%d: %p, next: %p, size: %d\n", i, p, p->next, p->size);
		prev = p;
	}
}

void add_to_free_chunks(Header* chunkp){
	Header *p;

    for (p = free_chunks; !(chunkp > p && chunkp < p->next); p = p->next)
        if (p >= p->next && (chunkp > p || chunkp < p->next))
            break;

    if(chunkp+chunkp->size == p->next){
    	chunkp->next = p->next->next;
    	chunkp->size += p->next->size;
    }
    else{
    	chunkp->next = p->next;
    }

    if(p+p->size == chunkp){
    	p->size += chunkp->size;
    	p->next = chunkp->next;
    }
    else{
    	p->next = chunkp;
    }

    free_chunks = p;
}

void* req_heap(){
	void* res;
	res = sbrk(REQ_HEAP_SIZE);
	if(res == (void*) -1){
		return NULL;
	}
	Header* chunkp;
	chunkp = (Header*) res;
	chunkp->size = REQ_HEAP_SIZE/sizeof(Header);
	add_to_free_chunks(chunkp);
	printf("Heap requested: %p\n", chunkp);
	return free_chunks;
}

void* mem_alloc(size_t units){
	units = (units + sizeof(Header) - 1) / sizeof(Header) + 1; 

	Header* prev;
	Header* p;

	prev = free_chunks;
	p = prev->next;
	while(1){

		if(p->size >= units){
			if(p->size == units){
				prev->next = p->next;
			}
			else{
				p->size -= units;
				p += p->size;
				p->size = units;
			}

			free_chunks = prev;
			if(used_chunks == NULL){
				used_chunks = p;
				p->next = p;
			}
			else{
				p->next = used_chunks->next;
				used_chunks->next = p;

			}

			return (void*) p+1;
		}

		if(p == free_chunks){
			p = req_heap();
			if(p == NULL){
				return NULL;
			}
		}
		prev = p;
		p = p ->next;
	}
	return NULL;
}

int main(int argc, char const *argv[])
{
	void* pointer = mem_alloc(32);
	printf("%p\n", pointer);
	print_chunks(&base);
	void* pointer2 = mem_alloc(32);
	printf("%p\n", pointer2);
	print_chunks(&base);
	printf("used_chunks:\n");
	print_chunks(used_chunks);
}