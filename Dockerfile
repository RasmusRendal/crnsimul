FROM alpine

RUN apk add gcc g++ libstdc++ make flex bison libc-dev gtest gtest-dev bash

RUN mkdir /chem

WORKDIR /chem

COPY Makefile src/GCRN/driver.cc src/GCRN/driver.hh src/GCRN/GCRNParse.yy src/GCRN/GCRNScan.l src/GCRN/input src/GCRN/main.cc ./
RUN mkdir tests
COPY tests/* ./tests/

RUN sh -c "ls && make"
