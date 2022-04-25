//
// Created by pengc on 2022/4/24.
//

#include <aw_mutex.h>
#include <windows.h>
#include <aw_memory.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////
struct aw_mutex_t {
    CRITICAL_SECTION mutex;
};

////////////////////////////////////////////////////////////////////////////////
////
aw_mutex_t * aw_mutex_new(){
    aw_mutex_t * p;
    AW_NEW(p);
    assert(p);

    InitializeCriticalSection(&p->mutex);

    return p;
}

void aw_mutex_free(aw_mutex_t** pMutex){
    assert(pMutex && *pMutex);
    DeleteCriticalSection(&(*pMutex)->mutex);
}

void aw_mutex_lock(aw_mutex_t* mutex){
    assert(mutex);
    EnterCriticalSection(&mutex->mutex);
}

void aw_mutex_unlock(aw_mutex_t *mutex){
    assert(mutex);
    LeaveCriticalSection(&mutex->mutex);
}

void* aw_mutex_raw(aw_mutex_t *mutex){
    assert(mutex);
    return &mutex->mutex;
}