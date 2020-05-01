FROM alpine

RUN apk add clang boost-iostreams boost-dev libstdc++ cmake make build-base gcc flex bison libc-dev gtest gtest-dev bash gcovr glfw-dev glew-dev freetype-dev glm-dev mesa mesa-dev fontconfig-dev

RUN mkdir /chem

WORKDIR /chem

COPY CMakeLists.txt ./
COPY gencov.sh ./
COPY src ./src/
COPY submodules ./submodules/
RUN mkdir tests
COPY tests/* ./tests/

RUN sh -c "cmake -DCMAKE_BUILD_TYPE=Debug . && make && ./bin/tests"
