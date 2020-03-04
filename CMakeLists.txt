cmake_minimum_required(VERSION 3.10)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set the project name
project(Chemilang VERSION 0.1)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(CRNParse src/CRN/CRNParse.yy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CRNParse.cc)
FLEX_TARGET(CRNScan src/CRN/CRNScan.l ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CRNScan.cc)
ADD_FLEX_BISON_DEPENDENCY(CRNScan CRNParse)

include_directories(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
include_directories(src)
include_directories(src/CRN)

file(GLOB SOURCES "src/CRN/*.cc")

# add the executable
add_executable(chemilang
	${SOURCES}
	${BISON_CRNParse_OUTPUTS}
	${FLEX_CRNScan_OUTPUTS}
	)
