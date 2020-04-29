NAME = find-arena-size

CC = clang
CXX = clang++
CFLAGS ?= -Wall

MACROS += -DTF_LITE_DISABLE_X86_NEON
CXXFLAGS += -std=c++11
CFLAGS += -I.
CFLAGS += -Isource
CFLAGS += -Iedge-impulse-sdk/
CFLAGS += -Iedge-impulse-sdk/tensorflow
CFLAGS += -Iedge-impulse-sdk/third_party
CFLAGS += -Iedge-impulse-sdk/third_party/flatbuffers
CFLAGS += -Iedge-impulse-sdk/third_party/flatbuffers/include
CFLAGS += -Iedge-impulse-sdk/third_party/flatbuffers/include/flatbuffers
CFLAGS += -Iedge-impulse-sdk/third_party/gemmlowp/
CFLAGS += -Iedge-impulse-sdk/third_party/gemmlowp/fixedpoint
CFLAGS += -Iedge-impulse-sdk/third_party/gemmlowp/internal
CFLAGS += -Iedge-impulse-sdk/third_party/ruy
CFLAGS += -Imodel-parameters
CFLAGS += -Iedge-impulse-sdk/porting

all: build

.PHONY: build clean

build:
	mkdir -p build
	$(CC) -c $(MACROS) $(CFLAGS) $(LFLAGS) edge-impulse-sdk/tensorflow/lite/c/common.c -o build/common.o
	$(CXX) $(MACROS) $(CXXFLAGS) $(CFLAGS) $(LFLAGS) source/*.cpp edge-impulse-sdk/tensorflow/lite/kernels/*.cc edge-impulse-sdk/tensorflow/lite/kernels/internal/*.cc edge-impulse-sdk/tensorflow/lite/micro/kernels/*.cc edge-impulse-sdk/tensorflow/lite/micro/*.cc edge-impulse-sdk/tensorflow/lite/micro/memory_planner/*.cc edge-impulse-sdk/tensorflow/lite/core/api/*.cc ./edge-impulse-sdk/dsp/memory.cpp edge-impulse-sdk/porting/posix/*.c* build/common.o -o build/$(NAME)

clean:
	rm $(NAME)
