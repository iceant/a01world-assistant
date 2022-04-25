//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_TYPES_H
#define INCLUDED_AW_TYPES_H

#ifndef INCLUDED_STDINT_H
#define INCLUDED_STDINT_H
#include <stdint.h>
#endif // INCLUDED_STDINT_H

#ifndef INCLUDED_STDBOOL_H
#define INCLUDED_STDBOOL_H
#include <stdbool.h>
#endif // INCLUDED_STDBOOL_H

////////////////////////////////////////////////////////////////////////////////
////
typedef int8_t aw_int8_t;
typedef int16_t aw_int16_t;
typedef int32_t aw_int32_t;
typedef int64_t aw_int64_t;

typedef uint8_t aw_uint8_t;
typedef uint16_t aw_uint16_t;
typedef uint32_t aw_uint32_t;
typedef uint64_t aw_uint64_t;

typedef size_t aw_size_t;

typedef intptr_t aw_intptr_t;
typedef uintptr_t aw_uintptr_t;

typedef bool aw_bool_t;

#define AW_TRUE 1
#define AW_FALSE 0

////////////////////////////////////////////////////////////////////////////////
////
#define AW_INT_MIN INT32_MIN
#define AW_INT_MAX INT32_MAX

#define AW_INT8_MIN INT8_MIN
#define AW_INT8_MAX INT8_MAX

#define AW_INT16_MIN INT16_MIN
#define AW_INT16_MAX INT16_MAX

#define AW_INT32_MIN INT32_MIN
#define AW_INT32_MAX INT32_MAX

#define AW_INT64_MIN INT64_MIN
#define AW_INT64_MAX INT64_MAX

#define AW_UINT8_MAX UINT8_MAX
#define AW_UINT16_MAX UINT16_MAX
#define AW_UINT32_MAX UINT32_MAX
#define AW_UINT64_MAX UINT64_MAX

#define AW_SIZE_MAX SIZE_MAX
#define AW_INTPTR_MIN INTPTR_MIN
#define AW_INTPTR_MAX INTPTR_MAX
#define AW_UINTPTR_MAX UINTPTR_MAX
#define AW_PTRDIFF_MIN PTRDIFF_MIN
#define AW_PTRDIFF_MAX PTRDIFF_MAX

#endif //INCLUDED_AW_TYPES_H
