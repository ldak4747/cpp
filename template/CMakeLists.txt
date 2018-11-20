cmake_minimum_required(VERSION 2.8)

project(data-access)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILECMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb")
set(_GLIBCXX_USE_NANOSLEEP ON)
set(CMAKE_BUILD_TYPE "Debug")

set(ROOT_DIR "/usr/local")

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GCC_BASE_DIR "${BASIC_DIR}/gcc7.3")
set(GTEST_BASE_DIR "${BASIC_DIR}/gtest/gtest-1.7.0")
set(GTEST_INCLUDE_DIR "${BASIC_DIR}/gtest/gtest-1.7.0/include")
set(CMAKE_C_COMPILER "${GCC_BASE_DIR}/bin/gcc")
set(CMAKE_CXX_COMPILER "${GCC_BASE_DIR}/bin/g++")

include_directories(${GTEST_INCLUDE_DIR})
link_directories(${GTEST_BASE_DIR})

set(BASE_SRC base.cc)
add_executable(base ${BASE_SRC})
target_link_libraries(base pthread)
target_link_libraries(base gtest)

set(SHARED_PTR_SRC SharedPtr.cc)
add_executable(SharedPtr ${SHARED_PTR_SRC})
target_link_libraries(SharedPtr pthread)
