#ifndef EI_FIND_TENSOR_SIZE_H_
#define EI_FIND_TENSOR_SIZE_H_

#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

static tflite::MicroErrorReporter micro_error_reporter;
static tflite::ErrorReporter* error_reporter = &micro_error_reporter;

static int test(const tflite::Model* model, size_t arena_size) {
    // Create an area of memory to use for input, output, and intermediate arrays.
    // Finding the minimum value for your model may require some trial and error.
    uint8_t *tensor_arena = (uint8_t*)malloc(arena_size);
    if (!tensor_arena) {
        return -2;
    }

    tflite::ops::micro::AllOpsResolver resolver;

    // Build an interpreter to run the model with.
    tflite::MicroInterpreter interpreter(model, resolver, tensor_arena, arena_size, error_reporter);

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter.AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        free(tensor_arena);
        return -1;
    }

    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter.Invoke();
    if (invoke_status != kTfLiteOk) {
        free(tensor_arena);
        return -1;
    }

    free(tensor_arena);

    return 0;
}

int find_arena_size(char *tflite_buffer, size_t a_low, size_t a_high) {
    const tflite::Model* model = tflite::GetModel(tflite_buffer);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        printf(
            "Model provided is schema version %d not equal "
            "to supported version %d.\n",
            model->version(), TFLITE_SCHEMA_VERSION);
        return -1;
    }

    size_t low = a_low;
    size_t high = a_high;
    size_t curr = low + ((high - low) / 2);
    size_t last_success = high;
    size_t steps = 0;

    while (steps < 100) {
        ++steps;
        int r = test(model, curr);

        if (r == 0) {
            last_success = curr;
        }

        if (r == -1) { // too low
            low = curr;
            curr = low + ((high - low) / 2);
        }
        else {
            high = curr;
            curr = low + ((high - low) / 2);
        }

        if (low == curr || high == curr) {
            break;
        }
    }

    if (last_success == a_high || steps >= 99) {
        return -1;
    }

    return last_success;
}

#endif // EI_FIND_TENSOR_SIZE_H_
