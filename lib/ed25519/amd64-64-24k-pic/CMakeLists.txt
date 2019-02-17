include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.[cs])

ed25519_add_library(amd64-64-24k-pic EDIMPL
  SOURCES ${sources}
  IS_SUPPORTED ${CAN_BUILD_AMD64}
  )

## linux only solution
#if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#  if(EXISTS ${CMAKE_SOURCE_DIR}/linker_exportmap)
#    set(_map ${CMAKE_SOURCE_DIR}/linker_exportmap)
#  else()
#    set(_map ${CMAKE_INSTALL_LIBDIR}/cmake/ed25519/linker_exportmap)
#  endif()
#
#  ed25519_target_link_libraries(amd64-64-24k-pic
#    "-Wl,--version-script=${_map}"
#    )
#endif ()
