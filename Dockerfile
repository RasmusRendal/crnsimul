FROM alpine

RUN apk add gcc g++ libstdc++ make flex bison libc-dev gtest gtest-dev

RUN mkdir /chem

WORKDIR /chem

COPY Makefile chem.l chem.y ./
RUN mkdir tests
COPY tests/* ./tests/

RUN sh -c "ls && make"
