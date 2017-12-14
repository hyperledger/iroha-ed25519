function(configedimpl target)
  set_target_properties(${target} PROPERTIES EXCLUDE_FROM_ALL TRUE)

  install(TARGETS ${LIB} EXPORT ed25519Config
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          )

  target_include_directories(${target} PUBLIC
          $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
          $<INSTALL_INTERFACE:include>
          )
endfunction()

if(EDIMPL STREQUAL "amd64-64-24k")
  add_subdirectory(amd64-64-24k)
elseif(EDIMPL STREQUAL "amd64-64-24k-pic")
  add_subdirectory(amd64-64-24k-pic)
elseif(EDIMPL STREQUAL "ref10")
  add_subdirectory(ref10)
else()
  message(FATAL_ERROR "No such EDIMPL implementation ${EDIMPL}")
endif()
