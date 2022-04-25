//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_THREAD_H
#define INCLUDED_AW_THREAD_H


#ifndef INCLUDED_AW_TYPES_H
#include <aw_types.h>
#endif // INCLUDED_AW_TYPES_H

////////////////////////////////////////////////////////////////////////////////
////
typedef struct aw_thread_t aw_thread_t;

typedef struct aw_thread_new_ops{
    union{
        void (*thread_fn)(void *);
        unsigned (*thread_ret_fn)(void*);
    };
    void *args;
    aw_size_t stack_size;
    void* platform_ops;
} aw_thread_new_ops;

////////////////////////////////////////////////////////////////////////////////
////
aw_thread_t *aw_thread_new(aw_thread_new_ops newOps);

void aw_thread_free(aw_thread_t ** pThread);

void aw_thread_join(aw_thread_t * thread);

aw_uint32_t aw_thread_self_id();

#endif //INCLUDED_AW_THREAD_H
