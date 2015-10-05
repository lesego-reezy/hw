/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"

#include <stdlib.h>

/* Your final implementation should comment out this macro. */
#define MM_USE_STUBS
#define BLK_SZ sizeof(struct s_block)
#define align4(x) ((((x) - 1) >> 2) << 2) + 4

void *foot = NULL;
size_t size;

s_block_ptr extend_heap(s_block_ptr last, size_t s)
{
    s_block_ptr block = sbrk(0);
    if(sbrk(s+BLK_SZ)== (void*) -1)
        return NULL;

    block->size = s;
    block->prev = last;
    block->next = NULL;
    block->free = 0;
    block->ptr  = block->data;
    if(last){
        last->next = block;
    }
    return block;
}
void split_block(s_block_ptr b, size_t s)
{
    s_block_ptr splitter;
    splitter = (s_block_ptr)(b->data + s);
    splitter->size = b->size - s - BLK_SZ;
    splitter->prev = b;
    splitter->next = b->next;
    splitter->free = 1;
    splitter->ptr = splitter->data;
    b->size = s;
    b->next = splitter;

    if(splitter->next){
        splitter->next->prev = splitter;
    }
}

s_block_ptr fusion(s_block_ptr b){
    if (b->prev != NULL && b->prev->free) {
        b->prev->next = b->next;
        b->prev->size = b->prev->size + BLK_SZ + b->size;
        return b->prev;
    }
    if (b->next != NULL && b->next->free) {
        b->next = b->next->next;
        b->size = b->size + BLK_SZ + b->next->size;
    }

    return b;
}

s_block_ptr get_block(void *p)
{
    char *pointer;
    pointer = p;
    pointer -= BLOCK_SIZE;
    p = pointer;
    return p;
}

void* mm_malloc(size_t size)
{
#ifdef MM_USE_STUBS
    return calloc(1, size);
#else
    s_block_ptr block, last;

    size_t s;

    s = align4(size);

    if(foot){
        last = foot;
        block = find_block(last, s);
        if(block){
            if((block->size - s) >= (BLOCK_SIZE + 4)) split_block(block, s);
            block->free = 0;
        }
        else{
            block = extend_heap(last, s);
            if(!block) return NULL;
        }
    }
    else{
        block = extend_heap(NULL, s);
        if(!block) return NULL;
        foot = block;
    }
    return block->data;
    //#error Not implemented.
#endif
}

void* mm_realloc(void* ptr, size_t size)
{
#ifdef MM_USE_STUBS
    return realloc(ptr, size);
#else
    if(size==0){
        free(ptr);
        return ptr;
    }
    if(!ptr)
        return malloc(size);
    t_block last;
    t_block b = valid_addr(ptr,&last);
    if(!b)
        return ptr;
    size_t s = align4(size);
    if(b->size >= s){
        if(b->size > s+BLOCK_SIZE+4)
            split_block(b,s);
        return ptr;
    }
    void *new = malloc(s);
    if(!new)
        return NULL;
    size_t i,s4,*sp,*newp;
    s4=s/(sizeof(size_t));
    sp=(size_t*)(p);
    newp=(size_t*)(new);
    for(i=0;i<s4;i++)
        newp[i]=sp[i];
    free(ptr);
    return new;
    //#error Not implemented.
#endif
}

void mm_free(void* ptr)
{
#ifdef MM_USE_STUBS
    free(ptr);
#else
    s_block_ptr b;
    if (valid_addr(ptr)){
        b = get_block(ptr);
        b->free = 1;
        if(b->prev && b->prev->free) b = fusion(b->prev);
        if(b->next){
            fusion(b);
        }
        else{
            if(b->prev) b->prev->next = NULL;
            else base = NULL;
            brk(b);
        }
    }
    //#error Not implemented.
#endif
}
