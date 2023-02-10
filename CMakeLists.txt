cmake_minimum_required(VERSION 3.0.0)
project(baa2sf2 VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 20)

find_package(Poco REQUIRED Foundation)

add_subdirectory(sf2cute)

add_executable(baa2sf2
  src/main.cpp

  src/AfcDecoder.cpp
  src/AudioArchiveLoader.cpp
  src/BasicInstrument.cpp
  src/BnkParser.cpp
  src/InstrumentBank.cpp
  src/PercussionSet.cpp
  src/utils.cpp
  src/WaveBank.cpp
  src/WaveGroup.cpp
  src/WaveHandle.cpp
  src/WsysParser.cpp
  src/WavePool.cpp
)

target_include_directories(baa2sf2 PRIVATE
  src
  sf2cute/include
)

target_link_libraries(
  baa2sf2
  PRIVATE Poco::Foundation
  PRIVATE sf2cute
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
