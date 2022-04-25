//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_VECTOR_H
#define INCLUDED_AW_VECTOR_H

////////////////////////////////////////////////////////////////////////////////
////
#ifndef INCLUDED_AW_TYPES_H
#include <aw_types.h>
#endif // INCLUDED_AW_TYPES_H

////////////////////////////////////////////////////////////////////////////////
////
typedef struct aw_vector_t aw_vector_t;

////////////////////////////////////////////////////////////////////////////////
////
aw_vector_t *aw_vector_new(aw_size_t capacity);

void aw_vector_free(aw_vector_t ** pVector);

void aw_vector_push_back(aw_vector_t *vector, void* data);

void aw_vector_set(aw_vector_t *vector, aw_size_t idx, void *data);

void* aw_vector_get(aw_vector_t *vector, aw_size_t idx);

aw_size_t aw_vector_capacity(aw_vector_t * vector);

aw_size_t aw_vector_size(aw_vector_t * vector);

aw_bool_t aw_vector_isEmpty(aw_vector_t * vector);

void* aw_vector_pop(aw_vector_t *vector);

#endif //INCLUDED_AW_VECTOR_H
