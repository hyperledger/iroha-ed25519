SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-funroll-loops -O3 -fomit-frame-pointer")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -O0 -fdiagnostics-color")


hunter_add_package(benchmark)
find_package(benchmark CONFIG REQUIRED)

macro(bench name)
  add_executable(benchmark-${name} benchmark.cpp)
  target_link_libraries(benchmark-${name}
    ${name}
    ${ARGN}
    benchmark::benchmark
    )
endmacro()

bench(
  ${EDIMPL}
  ${HASH}
  ${RANDOM}
)
