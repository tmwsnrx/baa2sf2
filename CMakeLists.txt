cmake_minimum_required(VERSION 3.0.0)
project(baa2sf2 VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

add_executable(baa2sf2 src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
