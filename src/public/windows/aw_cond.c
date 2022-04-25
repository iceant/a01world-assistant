//
// Created by pengc on 2022/4/24.
//

#include <aw_cond.h>
#include <aw_memory.h>
#include <windows.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////
struct aw_cond_t{
    CONDITION_VARIABLE conditionVariable;
};

////////////////////////////////////////////////////////////////////////////////
////

aw_cond_t * aw_cond_new(){
    aw_cond_t * p;
    AW_NEW(p);
    assert(p);
    InitializeConditionVariable(&p->conditionVariable);
    return p;
}

void aw_cond_free(aw_cond_t ** pAwConditionVariable){
    assert(pAwConditionVariable && *pAwConditionVariable);

    AW_FREE(*pAwConditionVariable);
}

void aw_cond_broadcast(aw_cond_t * conditionVariable){
    assert(conditionVariable);
    WakeAllConditionVariable(&conditionVariable->conditionVariable);
}

void aw_cond_signal(aw_cond_t * conditionVariable){
    assert(conditionVariable);
    WakeConditionVariable(&conditionVariable->conditionVariable);
}

void aw_cond_wait(aw_cond_t* conditionVariable, aw_mutex_t* mutex){
    assert(conditionVariable);
    assert(mutex);
    SleepConditionVariableCS(&conditionVariable->conditionVariable, aw_mutex_raw(mutex), INFINITE);
}

void aw_cond_waitForMilliSecond(aw_cond_t * cond, aw_mutex_t* mutex, aw_uint32_t milliseconds){
    assert(cond);
    assert(mutex);
    SleepConditionVariableCS(&cond->conditionVariable, aw_mutex_raw(mutex), milliseconds);
}