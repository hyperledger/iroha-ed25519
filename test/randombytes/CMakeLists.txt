macro(rngtest name)
  addtest(test-${name} random.cpp)
  target_link_libraries(test-${name} PUBLIC
    ${name}
    )
endmacro()

rngtest(rand_openssl)
rngtest(dev_random)
rngtest(dev_urandom)
