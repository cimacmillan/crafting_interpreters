#ifndef clox_hashmap_h
#define clox_hashmap_h

#include "memory.h"
#include "value.h"

struct lox_hashmap_entry {
    char_array key;
    lox_value value;
    uint32_t hash;
};

typedef struct lox_hashmap_entry lox_hashmap_entry;

LINKED_LIST_H(lox_hashmap_entry)

DYNAMIC_ARRAY_H(lox_hashmap_entry_linked_list)
typedef lox_hashmap_entry_linked_list_array lox_hashmap_table;

struct lox_hashmap {
    lox_hashmap_table table;
    int usage;
};

typedef struct lox_hashmap lox_hashmap;

void lox_hashmap_init(lox_hashmap *map);
void lox_hashmap_insert(lox_hashmap *map, char_array key, lox_value value);
void lox_hashmap_delete(lox_hashmap *map, char_array key);
bool lox_hashmap_contains(lox_hashmap *map, char_array key);
lox_value*lox_hashmap_get(lox_hashmap *map, char_array key);
void lox_hashmap_print(lox_hashmap *map);



#endif
