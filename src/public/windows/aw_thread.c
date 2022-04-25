//
// Created by pengc on 2022/4/24.
//

#include <aw_thread.h>
#include <aw_types.h>
#include <windows.h>
#include <aw_memory.h>
#include <process.h>
#include <aw_thread_windows.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
////
struct aw_thread_t {
    aw_uintptr_t thread_p;
    aw_uint32_t thread_addr;
};

////////////////////////////////////////////////////////////////////////////////
////
aw_thread_t *aw_thread_new(aw_thread_new_ops ops){
    aw_thread_t * thread;
    AW_NEW(thread);

    aw_thread_windows_ops * windowsOps;

    if(ops.platform_ops!=NULL){
        windowsOps = (aw_thread_windows_ops *)ops.platform_ops;
        thread->thread_p = _beginthreadex(windowsOps->security, ops.stack_size, ops.thread_ret_fn, ops.args, windowsOps->initflag, &thread->thread_addr);
        if(windowsOps->thrdaddr!=NULL){
            *(windowsOps->thrdaddr) = thread->thread_addr;
        }
    }else{
        thread->thread_p = _beginthreadex(NULL, ops.stack_size, ops.thread_ret_fn, ops.args, 0, &thread->thread_addr);
    }

    return thread;
}


void aw_thread_free(aw_thread_t ** pThread){
    assert(pThread && *pThread);
    aw_thread_t * thread = *pThread;
    if(thread->thread_p){
        CloseHandle((HANDLE)thread->thread_p);
    }
    AW_FREE(*pThread);
}

void aw_thread_join(aw_thread_t * thread){
    assert(thread);
    WaitForSingleObject((HANDLE)thread->thread_p, INFINITE);
}

aw_uint32_t aw_thread_self_id(){
    return GetCurrentThreadId();
}