FROM python:3.7.5-stretch

WORKDIR /app

# APT packages
RUN apt update && apt install -y zip lsb-release wget software-properties-common apt-transport-https

# Install LLVM for find-arena-size
RUN wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh && \
    ./llvm.sh 10

# Build the tflite-find-arena-size binary
RUN ln -s $(which clang-10) /usr/bin/clang && \
    ln -s $(which clang++-10) /usr/bin/clang++

COPY ./ .

# this segfaults in llvm10 on the linux box?!
RUN sed -i 's/MicroInterpreter::~MicroInterpreter() {/MicroInterpreter::~MicroInterpreter() { return;/g' ./edge-impulse-sdk/tensorflow/lite/micro/micro_interpreter.cc
RUN make && ln -s $PWD/build/find-arena-size /usr/bin/find-arena-size

ENTRYPOINT [ "/usr/bin/find-arena-size" ]
