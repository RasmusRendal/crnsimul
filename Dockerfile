FROM alpine

RUN apk add gcc g++ libstdc++ make flex bison libc-dev gtest gtest-dev bash

RUN mkdir /chem

WORKDIR /chem

COPY Makefile ./
COPY src ./src/
RUN mkdir tests
COPY tests/* ./tests/

RUN sh -c "ls && make"
