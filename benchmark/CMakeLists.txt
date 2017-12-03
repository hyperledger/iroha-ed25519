project(supercop_benchmark CXX)

SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-funroll-loops -O3 -fomit-frame-pointer")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -O0 -fdiagnostics-color")


macro(bench name)
  add_executable(benchmark-${name} benchmark.cpp)
  target_link_libraries(benchmark-${name}
      ${name}
      benchmark
      )
endmacro()

bench(ref)
bench(ref10)
bench(orlp-ed25519)

if(AMD64)
  bench(amd64-51-30k)
  bench(amd64-64-24k)
endif()

