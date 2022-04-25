//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_MUTEX_H
#define INCLUDED_AW_MUTEX_H

////////////////////////////////////////////////////////////////////////////////
////
typedef struct aw_mutex_t aw_mutex_t;

////////////////////////////////////////////////////////////////////////////////
////
aw_mutex_t * aw_mutex_new();

void aw_mutex_free(aw_mutex_t** pMutex);

void aw_mutex_lock(aw_mutex_t* mutex);

void aw_mutex_unlock(aw_mutex_t *mutex);

void* aw_mutex_raw(aw_mutex_t *mutex);

#endif //INCLUDED_AW_MUTEX_H
