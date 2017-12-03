find_package(PackageHandleStandardArgs)

include(ExternalProject)
set(EP_PREFIX "${PROJECT_SOURCE_DIR}/external")
set_directory_properties(PROPERTIES
    EP_PREFIX ${EP_PREFIX}
    )

find_package(gtest)
find_package(benchmark)

find_package(OpenSSL)
