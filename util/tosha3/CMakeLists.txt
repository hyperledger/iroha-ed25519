# this binary converts sign.input calculated with sha2 to exactly same sign.input but with sha3
cmake_minimum_required(VERSION 3.0)
project(ed25519_to_sha3 C)

find_package(ed25519 REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/..)

add_executable(tosha3
  tosha3.cpp
  )
target_link_libraries(tosha3
  ed25519
  )
target_compile_definitions(tosha3 PUBLIC
  -DTESTDATAIN_PATH=${CMAKE_SOURCE_DIR}/sign.input.with.sha2.txt
  -DTESTDATAOUT_PATH=${CMAKE_SOURCE_DIR}/sign.input.with.sha3.txt
  )
