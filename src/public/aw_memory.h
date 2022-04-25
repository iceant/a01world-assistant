//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_MEMORY_H
#define INCLUDED_AW_MEMORY_H

#ifndef INCLUDED_AW_TYPES_H
#include "aw_types.h"
#endif // INCLUDED_AW_TYPES_H

////////////////////////////////////////////////////////////////////////////////
////

void* aw_memory_malloc(aw_size_t nbytes, const char *file, int len);
void *aw_memory_calloc(aw_size_t count, aw_size_t nbytes, const char* file, int len);
void aw_memory_free(void *ptr, const char* file, int len);
void *aw_memory_resize(void* ptr, aw_size_t nbytes, const char* file, int len);
////////////////////////////////////////////////////////////////////////////////
////
#define AW_ALLOC(nbytes) aw_memory_malloc((nbytes), __FILE__, __LINE__)

#define AW_CALLOC(count, nbytes) aw_memory_calloc((count), (nbytes), __FILE__, __LINE__)

#define AW_RESIZE(ptr, nbytes) ((ptr)=aw_memory_resize((ptr), (nbytes), __FILE__, __LINE__))

#define AW_FREE(ptr) ((void)(aw_memory_free((ptr), __FILE__, __LINE__), (ptr)=0))

#define AW_NEW(p) ((p)=AW_ALLOC((aw_size_t)sizeof(*(p))))

#define AW_NEW0(p) ((p)=AW_CALLOC(1, (aw_size_t)sizeof(*(p))))



#endif //INCLUDED_AW_MEMORY_H
