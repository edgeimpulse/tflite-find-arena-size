#include <emscripten/bind.h>
#include "find_arena_size.h"

using namespace emscripten;

int emcc_find_arena_size(size_t tflite_buffer_raw, size_t a_low, size_t a_high) {
    char *tflite_buffer = (char*)tflite_buffer_raw;

    return find_arena_size(tflite_buffer, a_low, a_high);
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("find_arena_size", &emcc_find_arena_size, allow_raw_pointers());
}
