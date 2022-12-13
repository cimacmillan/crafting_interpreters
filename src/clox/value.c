#include "value.h"
#include "memory.h"
#include "vm.h"

DYNAMIC_ARRAY_IMPL(lox_value)
DYNAMIC_ARRAY_IMPL(char)

void lox_value_print(lox_value value) {
    if (IS_NUMBER(value)) {
        printf("%g", value.as.number);
    } else if (IS_BOOL(value)) {
        printf(AS_BOOL(value) ? "true" : "false");
    } else if (IS_NIL(value)) {
        printf("nil");
    } else if (IS_STRING(value)) {
        char_array_print(AS_STRING(value)->chars);
    } else if (IS_FUNCTION(value)) {
        printf("<fn ");
        char_array_print(AS_FUNCTION(value)->name);
        printf(">");
    } else if (IS_NATIVE_FUNCTION(value)) {
        printf("<native fn>");
    }
}

char_array char_array_from_const_char(const char *val) {
    int length = strlen(val);
    char_array result;
    char_array_init(&result);
    for (int i = 0; i < length; i++) {
        char_array_add(&result, val[i]);
    }
    return result;
}

void char_array_print(char_array array) {
    for (int i = 0; i < array.size; i++) {
        printf("%c", array.code[i]);
    }
}

bool char_array_is_equal(char_array a, char_array b) {
    if (a.size != b.size) return false;
    for (int i = 0; i < a.size; i++) {
        if (a.code[i] != b.code[i]) return false;
    }
    return true;
}

struct lox_heap_object_string* new_lox_string(char* from, int length) {
    lox_heap_object_string *obj = malloc(sizeof(lox_heap_object_string));
    obj->type = LOX_HEAP_OBJECT_TYPE_STRING;
    obj->next = NULL;
    char_array_init(&obj->chars);
    for (int i = 0; i < length; i++) {
        char_array_add(&obj->chars, from[i]);
    }
    return obj;
}

struct lox_heap_object_string* copy_lox_string(lox_heap_object_string* from) {
    lox_heap_object_string *obj = malloc(sizeof(lox_heap_object_string));
    obj->type = LOX_HEAP_OBJECT_TYPE_STRING;
    obj->next = NULL;
    char_array_init(&obj->chars);
    for (int i = 0; i < from->chars.size; i++) {
        char_array_add(&obj->chars, from->chars.code[i]);
    }
    return obj;
}

void free_obj_string(lox_heap_object_string *str) {
    char_array_free(&str->chars);
    free(str);
}

lox_heap_object_function* new_lox_function(char_array name) {
    lox_heap_object_function *obj = malloc(sizeof(lox_heap_object_function));
    obj->type = LOX_HEAP_OBJECT_TYPE_FUNCTION;
    obj->next = NULL;
    obj->arity = 0;
    obj->name = name;
    chunk_init(&obj->chunk);
    return obj;
}

lox_heap_object_native_function* new_lox_native_function(lox_native_fun fun) {
    lox_heap_object_native_function *obj = malloc(sizeof(lox_heap_object_native_function));
    obj->type = LOX_HEAP_OBJECT_TYPE_NATIVE_FUNCTION;
    obj->next = NULL;
    obj->func = fun;
    return obj;
}

void free_obj_func(lox_heap_object_function *func) {
    chunk_free(&func->chunk);
    free(func);;
}

void free_obj(lox_heap_object *obj) {
    switch (obj->type) {
        case LOX_HEAP_OBJECT_TYPE_STRING:
            free_obj_string((lox_heap_object_string*)obj);
        break;
        case LOX_HEAP_OBJECT_TYPE_FUNCTION:
            free_obj_func((lox_heap_object_function*)obj);
        break;
        case LOX_HEAP_OBJECT_TYPE_NATIVE_FUNCTION:
            free(obj);
        break;
    }
}
