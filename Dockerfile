FROM alpine

RUN apk add gcc make flex bison libc-dev bash

RUN mkdir /chem

COPY Makefile chem.l chem.y /chem/

RUN sh -c "cd /chem && make"
