#include "hashmap.h"

LINKED_LIST_IMPL(lox_hashmap_entry)
DYNAMIC_ARRAY_IMPL(lox_hashmap_entry_linked_list)

static int HASHMAP_INITIAL_SIZE = 64;

static uint32_t get_hash(char_array key) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < key.size; i++) {
        hash ^= (uint8_t)key.code[i];
        hash *= 16777619;
    }
    return hash;
}

void lox_hashmap_init(lox_hashmap *map) {
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
    uint32_t index = get_hash(key) % map->table.size;
    lox_hashmap_entry entry = { key, value };
    lox_hashmap_entry_linked_list *entry_list = &map->table.code[index];
    lox_hashmap_entry_linked_list_add(entry_list, entry);
    lox_hashmap_print(map);
}

void lox_hashmap_delete(lox_hashmap *map, char_array key) {
    (void)map;
    (void)key;
}

bool lox_hashmap_contains(lox_hashmap *map, char_array key) {
    uint32_t index = get_hash(key) % map->table.size;
    lox_hashmap_entry_linked_list *entry_list = map->table.code[index].next;
    while (entry_list != NULL) {
        if (char_array_is_equal(entry_list->value.key, key)) return true;
        entry_list = entry_list->next;
    }
    return false;
}

lox_value* lox_hashmap_get(lox_hashmap *map, char_array key) {
    uint32_t index = get_hash(key) % map->table.size;
    lox_hashmap_entry_linked_list *entry_list = map->table.code[index].next;
    while (entry_list != NULL) {
        if (char_array_is_equal(entry_list->value.key, key)) return &entry_list->value.value;
        entry_list = entry_list->next;
    }
    return NULL;
}

void lox_hashmap_entry_print(lox_hashmap_entry entry) {
    printf("('");
    char_array_print(entry.key);
    printf("', ");
    lox_value_print(entry.value);
    printf(") ");
}

void lox_hashmap_print(lox_hashmap *map) {
    printf("lox_hashmap\n");
    for (int i = 0; i < map->table.size; i++) {
        lox_hashmap_entry_linked_list entry_list = map->table.code[i];
        if (entry_list.next == NULL) continue;
        printf("%d] -> ", i);
        
        lox_hashmap_entry_linked_list *current = entry_list.next;
        while (current != NULL) {
            lox_hashmap_entry_print(current->value);
            current = current->next;
        }

        printf("\n");
    }
}

