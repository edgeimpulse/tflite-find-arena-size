#ifndef EI_FIND_TENSOR_SIZE_H_
#define EI_FIND_TENSOR_SIZE_H_

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

static tflite::MicroErrorReporter micro_error_reporter;
static tflite::ErrorReporter* error_reporter = &micro_error_reporter;

static size_t find_arena_size(char *tflite_buffer, size_t arena_size) {
    const tflite::Model* model = tflite::GetModel(tflite_buffer);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        printf(
            "Model provided is schema version %d not equal "
            "to supported version %d.\n",
            model->version(), TFLITE_SCHEMA_VERSION);
        return -1;
    }
    // Create an area of memory to use for input, output, and intermediate arrays.
    // Finding the minimum value for your model may require some trial and error.
    uint8_t *tensor_arena = (uint8_t*)malloc(arena_size);
    if (!tensor_arena) {
        return -2;
    }

    tflite::AllOpsResolver resolver;

    // Build an interpreter to run the model with.
    tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, arena_size, error_reporter);

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter.AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        free(tensor_arena);
        return -1;
    }

    // This obtains the number of bytes that are in use
    return interpreter.arena_used_bytes();
}

#endif // EI_FIND_TENSOR_SIZE_H_
