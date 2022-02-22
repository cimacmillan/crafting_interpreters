#include <stdio.h>
#include <stdlib.h>

struct point {
    int x;
    int y;
};

struct rect {
    point a;
    point b;
};

// Tree structure
struct node {
    node* left;
    node* right;
    int value;
};

// Map structure
struct entry {
    entry* nextEntry;
    char* text;
    char* replacement;
};

#define HASH_TABLE_SIZE 100

// Static limits this to this file
static entry* hashTable[HASH_TABLE_SIZE];

// When structs are large it can be faster to pass an address
point addPoint(point* a, point* b) {
    point x = {a->x + b->x, a->y + b->y};
    return x;
}

void printPoint(point* a) {
    printf("%d, %d \n", a->x, a->y);
}

node* createNode(int value) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void printTree(node* nodes) {
    printf("%d\n", nodes->value);
    if (nodes->left) {
        printTree(nodes->left);
    }
    if (nodes->right) {
        printTree(nodes->right);
    }
}

// Generate hash index from string
unsigned int hashFunction(char* string) {
    unsigned int hash = 0;
    for (int i = 0; string[i] != '\0'; i++) {
        hash = string[i] + (31 * hash);
    }
    return hash % HASH_TABLE_SIZE;
}

entry* createEntry(char* text, char* replacement) {
    entry* ent = (entry*)malloc(sizeof(entry));
    ent->nextEntry = NULL;
    ent->text = text;
    ent->replacement = replacement;
    return ent;
}

// ! Haven't addressed edge cases
void addEntry(char* text, char* replacement) {
    unsigned int hash = hashFunction(text);
    hashTable[hash] = createEntry(text, replacement);
}

char* getReplacement(char* text) {
    unsigned int hash = hashFunction(text);
    return hashTable[hash]->replacement;
}

void c6_main() {
    point x = {2, 5};

    printPoint(&x);

    rect y = {
        {2, 5},
        {6, 9}
    };

    printPoint(&y.a);
    printPoint(&y.b);

    point result = addPoint(&y.a, &y.b);
    printPoint(&result);

    printf("%d\n", sizeof(point));
    printf("%d\n", sizeof(rect));

    point points[] = {
        {2, 5},
        {6, 9}
    };
    point* pointer = &points[0];

    printf("%d address %X\n", pointer->x, pointer);

    pointer++;

    printf("%d address %X\n", pointer->x, pointer);

    node* root = createNode(10);
    root->left = createNode(20);
    root->left->left = createNode(30);
    root->left->right = createNode(40);
    printTree(root);

    printf("%d hash\n", hashFunction("Hello"));
    printf("%d hash\n", hashFunction("Goodbye"));

    addEntry("Hello", "Callum");
    addEntry("Goodbye", "is cool");

    printf("%s %s\n", getReplacement("Hello"), getReplacement("Goodbye"));

}

