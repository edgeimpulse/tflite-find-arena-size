#include <stdio.h>
#include <iostream>
#include <sstream>
#include "find_arena_size.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Requires one parameter (a path to a tflite file)\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Could not open '%s'\n", argv[1]);
        return 1;
    }

    int err = fseek(f, 0, SEEK_END);
    if (err != 0) {
        printf("Failed to seek end of file (%d)\n", err);
        return 1;
    }

    long file_len = ftell(f);
    rewind(f);

    char *tflite_buffer = (char*)malloc(file_len);
    if (!tflite_buffer) {
        printf("Failed to allocate tflite buffer (%lu bytes)\n", file_len);
        return 1;
    }

    size_t bytes_read = fread(tflite_buffer, 1, file_len, f);
    if (bytes_read != file_len) {
        printf("Failed to read tflite file\n");
        return 1;
    }

    size_t arena_size = find_arena_size(tflite_buffer, 4 * 1024 * 1024);
    printf("{\"arena_size\": %zu}\n", arena_size);
    if (arena_size < 0) {
        return 1;
    }
}
