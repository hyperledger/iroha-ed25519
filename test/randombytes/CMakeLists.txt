macro(rngtest name)
  addtest(test-${name} random.cpp)
  target_link_libraries(test-${name} ${name})
endmacro()

rngtest(${RANDOM})
