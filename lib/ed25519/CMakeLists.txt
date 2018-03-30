if(CAN_BUILD_AMD64)
  add_subdirectory(amd64-64-24k)
  add_subdirectory(amd64-64-24k-pic)
else()
  message(STATUS "amd64-64-24k targets are not available on your platform")
endif()

add_subdirectory(ref10)
