#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main (int argc, const char** argv) {
    (void)argc;
    (void)argv;

    lox_vm_init();

    lox_chunk chunk;
    chunk_init(&chunk);

    uint8_t ten = chunk_add_constant(&chunk, 10);
    chunk_add_code(&chunk, OP_CONSTANT, 123);
    chunk_add_code(&chunk, ten, 123);

    uint8_t twelve = chunk_add_constant(&chunk, 12);
    chunk_add_code(&chunk, OP_CONSTANT, 123);
    chunk_add_code(&chunk, twelve, 123);
    
    chunk_add_code(&chunk, OP_NEGATE, 123);

    chunk_add_code(&chunk, OP_RETURN, 123);

    // disassemble_chunk(&chunk, "test");

    lox_vm_run(&chunk);

    lox_vm_free();
    chunk_free(&chunk);

    return 0;
}

