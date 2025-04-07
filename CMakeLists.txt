cmake_minimum_required(VERSION 3.20)
project(wof)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cc")

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})

add_executable(wofc ${SOURCES})
target_link_libraries(wofc LLVM)
target_include_directories(wofc PUBLIC "include" ${LLVM_INCLUDE_DIRS})
target_precompile_headers(wofc PUBLIC include/pch.hpp)
