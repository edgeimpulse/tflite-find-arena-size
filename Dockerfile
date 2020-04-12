FROM python:3.7.5-stretch

WORKDIR /app

# APT packages
RUN apt update && apt install -y zip

# Install Emscripten
RUN git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    ./emsdk install 1.39.7 && \
    ./emsdk activate 1.39.7

RUN echo "PATH=$PATH:/app/emsdk:/app/emsdk/node/12.9.1_64bit/bin:/app/emsdk/upstream/emscripten" > ~/.bashrc

COPY . ./

CMD ["/bin/bash", "-c", "source /root/.bashrc && make -f Makefile.emcc"]
