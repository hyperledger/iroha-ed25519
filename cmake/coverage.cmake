set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "--coverage ${CMAKE_C_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "--coverage ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "--coverage ${CMAKE_EXE_LINKER_FLAGS}")

set(REPORT_DIR ${CMAKE_BINARY_DIR}/reports)
file(MAKE_DIRECTORY ${REPORT_DIR})
include(cmake/analysis.cmake)
