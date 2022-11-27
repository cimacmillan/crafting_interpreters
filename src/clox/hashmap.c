#include "hashmap.h"

LINKED_LIST_IMPL(lox_hashmap_entry)
DYNAMIC_ARRAY_IMPL(lox_hashmap_entry_linked_list)

static int HASHMAP_INITIAL_SIZE = 64;

void lox_hashmap_init(lox_hashmap *map) {
    (void)map;
    lox_hashmap_entry_linked_list_array_init(&map->table);
    lox_hashmap_entry_linked_list_array_resize(&map->table, HASHMAP_INITIAL_SIZE);
}

void lox_hashmap_insert(lox_hashmap *map, char_array key, lox_value value) {
    (void)map;
    (void)key;
    (void)value;
    printf("insert: ");
    char_array_print(key);
    printf("\n");
}

void lox_hashmap_delete(lox_hashmap *map, char_array key) {
    (void)map;
    (void)key;
}

bool lox_hashmap_contains(lox_hashmap *map, char_array key) {
    (void)map;
    (void)key;
    return false;
}

lox_value* lox_hashmap_get(lox_hashmap *map, char_array key) {
    (void)map;
    (void)key;
     printf("get: ");
    char_array_print(key);
    printf("\n");
    return NULL;
}
