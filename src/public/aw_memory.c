//
// Created by pengc on 2022/4/24.
//

#include "aw_memory.h"
#include <stdlib.h>
#include <assert.h>
//#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
////
void* aw_memory_malloc(aw_size_t nbytes, const char *file, int line){
    void* ptr;
    assert(nbytes>0);
    ptr = malloc(nbytes);
//    printf("%d %s:%d\n", nbytes, file, line);
    assert(ptr);
    return ptr;
}
void *aw_memory_calloc(aw_size_t count, aw_size_t nbytes, const char* file, int line){
    void *ptr;
    assert(count>0);
    assert(nbytes>0);
    ptr = calloc(count, nbytes);
    assert(ptr);
    return ptr;
}

void aw_memory_free(void *ptr, const char* file, int line){
    if(ptr) free(ptr);
}

void *aw_memory_resize(void* ptr, aw_size_t nbytes, const char* file, int line){
    assert(ptr);
    assert(nbytes>0);
    ptr = realloc(ptr, nbytes);
    assert(ptr);
    return ptr;
}