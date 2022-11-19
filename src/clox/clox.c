#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static char* readFile(const char* file_name) {
    FILE* file_handle = fopen(file_name, "rb");
    fseek(file_handle, 0L, SEEK_END);
    size_t fileSize = ftell(file_handle);
    rewind(file_handle);

    char* buffer = (char*)malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file_handle);
    buffer[bytesRead] = '\0';

    fclose(file_handle);
    return buffer;
}

lox_vm_result runFile(const char* file) {
    char* source = readFile(file);
    lox_vm_result result = interpret(source);
    free(source);
    return result;
}

int main (int argc, const char** argv) {
    if (argc != 2) {
        printf("Expecting 2 arguments\n");
    }

    return runFile(argv[1]);
}

