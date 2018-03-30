find_package(PackageHandleStandardArgs)

include(ExternalProject)
set(EP_PREFIX "${PROJECT_SOURCE_DIR}/external")
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})

if(TESTING)
  find_package(gtest)
endif()
if(BENCHMARKING)
  find_package(benchmark)
endif()

find_package(OpenSSL)
