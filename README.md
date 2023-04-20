> **DEPRECATED: This is a very old repo. The Edge Impulse Python SDK has functions to profile models, incl. finding arena size when running under TensorFlow Lite Micro (both w/ CMSIS-NN and when running reference kernels). See https://docs.edgeimpulse.com/docs/edge-impulse-python-sdk/overview#profile**

# Find TensorFlow Lite Arena Size

TensorFlow Lite for microcontrollers requires an arena size to be specified, but there's no quick way of calculating the minimum size of the arena. This repository contains an application which loads a tflite model and uses a divide and conquer algorithm to find the optimal size. The size of the arena is dependent on the target architecture, so it's advisable to run this application on the same platform as where you want to deploy the model. What we've found so far is that the arena size on a 64-bit computer (e.g. under macOS) is always bigger than on 32-bits.

## Example outputs

Using float32 models (non-quantized) with TensorFlow Lite 2.1.

**Multi-layer perceptron with two hidden layers**

| Environment  | Arena size |
| ------------- | ------------- |
| macOS Catalina  | 2272 bytes  |
| Mbed OS (Cortex-M4F) | 1392 bytes |

**1D Convolutional neural net with two hidden layers**

| Environment  | Arena size |
| ------------- | ------------- |
| macOS Catalina  | 26400 bytes  |
| Mbed OS (Cortex-M4F) | 24896 bytes |

## Cloning this repository

This clones the repository and initializes all submodules:

```
$ git clone --recurse-submodules https://github.com/edgeimpulse/tflite-find-arena-size.git
```

## Building locally (Docker)

1. Install Docker.
1. Build the application:

    ```
    $ docker build -t tflite-find-arena-size .
    ```

1. Place a tflite file in this folder and name it `trained.tflite`, and run:

    ```
    $ docker run --rm -v $PWD:/data tflite-find-arena-size /data/trained.tflite
    arena_size is 1420
    ```

## Building locally (native)

1. On Linux: Install LLVM 9 ([install instructions](https://apt.llvm.org/)). The default `cmake` version on macOS Catalina already works.
1. Build the application:

    ```
    $ make
    ```

1. Run the application and pass in a tflite file:

    ```
    $ ./build/find-arena-size some-tflite-file.tflite
    {"arena_size": 291872}
    ```
