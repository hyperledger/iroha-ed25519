if(TESTING)
  # https://docs.hunter.sh/en/latest/packages/pkg/GTest.html
  hunter_add_package(GTest)
  find_package(GTest CONFIG REQUIRED)
endif()

if(BENCHMARKING)
  # https://docs.hunter.sh/en/latest/packages/pkg/benchmark.html
  hunter_add_package(benchmark)
  find_package(benchmark CONFIG REQUIRED)
endif()
