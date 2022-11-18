#include "common.h"
#include "chunk.h"
#include "debug.h"

int main (int argc, const char** argv) {
    (void)argc;
    (void)argv;
    printf("Hello World\n");

    lox_chunk chunk;
    chunk_init(&chunk);

    uint8_t ten = chunk_add_constant(&chunk, 10);

    chunk_add_code(&chunk, OP_RETURN);
    chunk_add_code(&chunk, OP_CONSTANT);
    chunk_add_code(&chunk, ten);

    disassemble_chunk(&chunk, "test");

    chunk_free(&chunk);

    return 0;
}

