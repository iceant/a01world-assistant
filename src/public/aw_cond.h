//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_COND_H
#define INCLUDED_AW_COND_H

#ifndef INCLUDED_AW_MUTEX_H
#include <aw_mutex.h>
#endif // INCLUDED_AW_MUTEX_H

#ifndef INCLUDED_AW_TYPES_H
#include <aw_types.h>
#endif // INCLUDED_AW_TYPES_H
////////////////////////////////////////////////////////////////////////////////
////
typedef struct aw_cond_t aw_cond_t;

////////////////////////////////////////////////////////////////////////////////
////

aw_cond_t * aw_cond_new();

void aw_cond_free(aw_cond_t ** pAwConditionVariable);

void aw_cond_broadcast(aw_cond_t * conditionVariable);

void aw_cond_signal(aw_cond_t * conditionVariable);

void aw_cond_wait(aw_cond_t* conditionVariable, aw_mutex_t* mutex);

void aw_cond_waitForMilliSecond(aw_cond_t * cond, aw_mutex_t* mutex, aw_uint32_t milliseconds);
#endif //INCLUDED_AW_COND_H
