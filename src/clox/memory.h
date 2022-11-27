#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"


#define DYNAMIC_ARRAY_H(type) \
    typedef struct { \
        type *code; \
        int size; \
        int capacity; \
    } type##_array; \
    void type##_array_init(type##_array *array); \
    void type##_array_add(type##_array *array, type code); \
    type type##_array_pop(type##_array *array); \
    void type##_array_free(type##_array *array); 

#define DYNAMIC_ARRAY_IMPL(type) \
    void type##_array_init(type##_array *array) { \
        array->code = NULL; \
        array->size = 0; \
        array->capacity = 0; \
    } \
    void type##_array_add(type##_array *array, type code) { \
        if (array->capacity <= array->size + 1) { \
            int old_capacity = array->capacity; \
            array->capacity = ARRAY_CAPACITY_GROW(array->capacity); \
            array->code = ARRAY_GROW(array->code, type, old_capacity, array->capacity); \
        } \
        array->code[array->size] = code; \
        array->size++; \
    } \
    type type##_array_pop(type##_array *array) { \
        array->code[array->size]; \
        array->size--; \
        return array->code[array->size]; \
    } \
    void type##_array_free(type##_array *array) { \
        int old_capacity = array->capacity; \
        ARRAY_FREE(array->code, type, old_capacity); \
        array->capacity = 0; \
        array->size = 0; \
    }

#define ARRAY_CAPACITY_GROW(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define ARRAY_GROW(array, type, old_capacity, new_capacity) \
    (type*)reallocate(array, sizeof(type) * old_capacity, sizeof(type) * new_capacity)

#define ARRAY_FREE(array, type, old_capacity) \
    (type*)reallocate(array, sizeof(type) * old_capacity, 0)

void* reallocate(void* pointer, size_t before, size_t after);

#define LINKED_LIST_H(type) \
    struct type##_linked_list { \
            type value; \
            struct type##_linked_list *next; \
    }; \
    typedef struct type##_linked_list type##_linked_list; \
    void type##_linked_list_init(type##_linked_list *array); \
    void type##_linked_list_add(type##_linked_list *array, type value); \
    void type##_linked_list_free(type##_linked_list *array); 


#define LINKED_LIST_IMPL(type) \
    void type##_linked_list_init(type##_linked_list *array) { \
        array->next = NULL; \
    } \
    void type##_linked_list_add(type##_array *array, type value) { \
        if (array->next) { \
            type##_linked_list_add(array->next, value); \
            return; \
        } \
        array->next = malloc(sizeof(type##_linked_list)); \
        type##_linked_list_init(array->next); \
        array->next->value = value; \
    } \
    void type##_linked_list_free(type##_array *array) { \
        if (array->next) { \
            type##_linked_list_free(array->next); \
        } \
        free(array->next); \
        \
    }

#endif
