project(supercop_benchmark CXX)

SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-funroll-loops -O3 -fomit-frame-pointer")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -O0 -fdiagnostics-color")


macro(bench name)
  add_executable(benchmark-${name} benchmark.cpp)
  target_link_libraries(benchmark-${name}
      ${name}
      ${ARGN}
      benchmark
      )
endmacro()

bench(
    ${EDIMPL}
    ${HASH}
    ${RANDOM}
)


