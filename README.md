# Find TensorFlow Lite Arena Size

TensorFlow Lite for microcontrollers requires an arena size to be specified, but there's no quick way of calculating the minimum size of the arena. This repository contains an application which loads a tflite model and uses a divide and conquer algorithm to find the optimal size. The size of the arena is dependent on the target architecture, so it's advisable to run this application on the same platform as where you want to deploy the model. What we've found so far is that the arena size on a 64-bit computer (e.g. under macOS) is always bigger than on 32-bits. There's also a cross-compilation script available which builds a WebAssembly version of this application, which you can use from any Node.js or web application. It seems that the WebAssembly version approximates 32-bit embedded systems pretty well.

## Example outputs

Using float32 models (non-quantized) with TensorFlow Lite 2.1.

**Multi-layer perceptron with two hidden layers**

| Environment  | Arena size |
| ------------- | ------------- |
| macOS Catalina  | 2264 bytes  |
| Node.js (WebAssembly)  | 1420 bytes  |
| Mbed OS (Cortex-M4F) | 1392 bytes |

**1D Convolutional neural net with two hidden layers**

| Environment  | Arena size |
| ------------- | ------------- |
| macOS Catalina  | 26400 bytes  |
| Node.js (WebAssembly)  | 1420 bytes  |
| Mbed OS (Cortex-M4F) | 1392 bytes |

## Cloning this repository

This clones the repository and initializes all submodules:

```
$ git clone --recurse-submodules https://github.com/edgeimpulse/tflite-find-arena-size.git
```

## Building locally (native)

1. On Linux: Install LLVM 9 ([install instructions](https://apt.llvm.org/)). The default `cmake` version on macOS Catalina already works.
1. Build the application:

    ```
    $ make -f Makefile.tflite
    ```

1. Run the application and pass in a tflite file:

    ```
    $ ./build/find-arena-size some-tflite-file.tflite
    arena_size is 2264
    ```

## Building locally (WebAssembly)

1. Install Docker.
1. Build the application:

    ```
    $ docker build -t tflite-find-arena-size .
    $ docker run -v $PWD/build:/app/build --rm tflite-find-arena-size
    ```

1. Run the application and pass in a tflite file:

    ```
    $ node emcc/test.js some-tflite-file.tflite
    arena_size is 1420
    ```

## Installing (npm)

1. Add the package to your Node.js application via:

    ```
    $ npm install tflite-find-arena-size --save
    ```

1. Use the package as usual:

    ```js
    const FindArena = require('tflite-find-arena-size');
    const fs = require('fs');

    (async () => {
        let fa = new FindArena();
        await fa.init();

        let tflite = await fs.promises.readFile('some-tflite-file.tflite');
        let size = await fa.findArenaSize(tflite, 1 * 1024, 128 * 1024);
        console.log('arena size is', size);
    })();
    ```
