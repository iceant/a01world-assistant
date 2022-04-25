//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_GETTIMEOFDAY_H
#define INCLUDED_GETTIMEOFDAY_H

#ifndef INCLUDED_WINDOWS_H
#define INCLUDED_WINDOWS_H
#include <windows.h>
#endif // INCLUDED_WINDOWS_H

//struct timezone {
//    int tz_minuteswest;
//    int tz_dsttime;
//};


int gettimeofday(struct timeval *tv, struct timezone *tz);

#endif //INCLUDED_GETTIMEOFDAY_H
