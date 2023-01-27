cmake_minimum_required(VERSION 3.0.0)
project(baa2sf2 VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

find_package(Poco REQUIRED Foundation)

add_executable(baa2sf2
  src/main.cpp

  src/AudioArchiveLoader.cpp
  src/BnkParser.cpp
  src/utils.cpp
)

target_link_libraries(
  baa2sf2
  PRIVATE Poco::Foundation
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
