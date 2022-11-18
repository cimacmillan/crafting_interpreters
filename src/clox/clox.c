#include "common.h"
#include "chunk.h"
#include "debug.h"

int main (int argc, const char** argv) {
    (void)argc;
    (void)argv;
    printf("Hello World\n");

    Chunk chunk;
    chunk_init(&chunk);
    chunk_add(&chunk, OP_RETURN);
    chunk_add(&chunk, OP_RETURN);
    chunk_add(&chunk, OP_RETURN);
    chunk_add(&chunk, OP_RETURN);

    disassemble_chunk(&chunk, "test");

    chunk_free(&chunk);

    return 0;
}

