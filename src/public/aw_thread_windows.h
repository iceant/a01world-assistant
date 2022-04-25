//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_THREAD_WINDOWS_H
#define INCLUDED_AW_THREAD_WINDOWS_H

////////////////////////////////////////////////////////////////////////////////
////
typedef struct aw_thread_windows_ops{
    void* security;
    unsigned initflag;
    unsigned * thrdaddr;
}aw_thread_windows_ops;

#endif //INCLUDED_AW_THREAD_WINDOWS_H
