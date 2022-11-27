#include "hashmap.h"

LINKED_LIST_IMPL(lox_hashmap_entry)
DYNAMIC_ARRAY_IMPL(lox_hashmap_entry_linked_list)

static int HASHMAP_INITIAL_SIZE = 2;
static float HASHMAP_USAGE_TO_RESIZE = 0.5f;

static uint32_t get_hash(char_array key) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < key.size; i++) {
        hash ^= (uint8_t)key.code[i];
        hash *= 16777619;
    }
    return hash;
}

static void lox_hashmap_enlarge(lox_hashmap *map) {    
    lox_hashmap new_map;
    lox_hashmap_init(&new_map);
    lox_hashmap_entry_linked_list_array_resize(&new_map.table, map->table.size * 2);
    for (int i = 0; i < map->table.size * 2; i++) {
        lox_hashmap_entry_linked_list_init(&map->table.code[i]);
    }
    for (int i = 0; i < map->table.size; i++) {
        lox_hashmap_entry_linked_list *entry_list = map->table.code[i].next;
        while (entry_list != NULL) {
            lox_hashmap_insert(&new_map, entry_list->value.key, entry_list->value.value);
            lox_hashmap_entry_linked_list *next = entry_list->next;
            free(entry_list);
            entry_list = next;
        }
    }
    lox_hashmap_entry_linked_list_array_free(&map->table);
    *map = new_map;
}

void lox_hashmap_init(lox_hashmap *map) {
    lox_hashmap_entry_linked_list_array_init(&map->table);
    lox_hashmap_entry_linked_list_array_resize(&map->table, HASHMAP_INITIAL_SIZE);
    for (int i = 0; i < HASHMAP_INITIAL_SIZE; i++) {
        lox_hashmap_entry_linked_list_init(&map->table.code[i]);
    }
    map->usage = 0;
}

void lox_hashmap_insert(lox_hashmap *map, char_array key, lox_value value) {
    uint32_t hash = get_hash(key);
    uint32_t index = hash % map->table.size;
    lox_hashmap_entry entry = { key, value, hash };
    lox_hashmap_entry_linked_list *entry_list = &map->table.code[index];
    if (entry_list->next == NULL) {
        map->usage++;
    }
    lox_hashmap_entry_linked_list_add(entry_list, entry);
    float size = (float)map->table.size;
    float usage = (float)map->usage;
    if (usage / size > HASHMAP_USAGE_TO_RESIZE) {
        lox_hashmap_enlarge(map);
    }
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

