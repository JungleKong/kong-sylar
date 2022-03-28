cmake_minimum_required(VERSION 2.8)
project(sylar)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

set(LIB_SRC
        src/log/log.cpp
        src/utils/util.cpp
        )

add_library(sylar SHARED ${LIB_SRC})
add_executable(test tests/test.cpp)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)
# INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
# INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(test sylar)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)