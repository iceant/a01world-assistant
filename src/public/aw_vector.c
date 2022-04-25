//
// Created by pengc on 2022/4/24.
//

#include <aw_vector.h>
#include <aw_memory.h>
#include <assert.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
////
struct aw_vector_t{
    aw_size_t capacity;
    aw_size_t size;
    void** array;
};

#define AW_VECTOR_GROWTH_FACTOR (2)

////////////////////////////////////////////////////////////////////////////////
////

aw_vector_t *aw_vector_new(aw_size_t capacity){
    aw_vector_t * p;
    assert(capacity>0);

    AW_NEW(p);
    assert(p);

    p->capacity=capacity;
    p->size = 0;
    p->array = AW_ALLOC(capacity * sizeof(*(p->array)));
    assert(p->array);

    return p;
}

void aw_vector_free(aw_vector_t ** pVector){
    assert(pVector && *pVector);
    AW_FREE((*pVector)->array);
    AW_FREE(*pVector);
}

void aw_vector_push_back(aw_vector_t *vector, void* data){
    assert(vector);
    if(vector->size>=vector->capacity){
        aw_size_t new_capacity = vector->capacity * AW_VECTOR_GROWTH_FACTOR;
        assert(new_capacity <= AW_SIZE_MAX);
        void * new_array = AW_ALLOC(new_capacity * sizeof(*(vector->array)));
        assert(new_array);
        memcpy(new_array, vector->array, vector->size * sizeof (*(vector->array)));
        AW_FREE(vector->array);
        vector->array = new_array;
        vector->capacity=new_capacity;
    }
    vector->array[vector->size] = data;
    vector->size++;
}

void aw_vector_set(aw_vector_t *vector, aw_size_t idx, void *data){
    assert(vector);
    assert(idx>-1 && idx<vector->size);
    vector->array[idx]=data;
}



void* aw_vector_get(aw_vector_t *vector, aw_size_t idx){
    assert(vector);
    assert(idx>=0 && idx<vector->size);
    return vector->array[idx];
}

aw_size_t aw_vector_capacity(aw_vector_t * vector){
    assert(vector);
    return vector->capacity;
}

aw_size_t aw_vector_size(aw_vector_t * vector){
    assert(vector);
    return vector->size;
}

aw_bool_t aw_vector_isEmpty(aw_vector_t * vector){
    assert(vector);
    return vector->size==0;
}

void* aw_vector_pop(aw_vector_t *vector){
    assert(vector);
    if(vector->size<1) return NULL;
    void* data = vector->array[vector->size-1];
    vector->size--;
    return data;
}