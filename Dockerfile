FROM alpine

RUN apk add clang boost-iostreams boost-dev libstdc++ cmake make build-base gcc flex bison libc-dev gtest gtest-dev bash

RUN mkdir /chem

WORKDIR /chem

COPY CMakeLists.txt ./
COPY src ./src/
COPY submodules ./submodules/
RUN mkdir tests
COPY tests/* ./tests/

RUN sh -c "ls && cmake . && make && ./bin/tests"
