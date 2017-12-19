macro(rngtest name)
  addtest(test-${name} random.cpp $<TARGET_OBJECTS:${name}>)
endmacro()

rngtest(${RANDOM})
