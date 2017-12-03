macro(rngtest name)
  addtest(test-${name} random.cpp)
  target_link_libraries(test-${name} ${name})
endmacro()

if(OPENSSL_FOUND)
  rngtest(rand_openssl)
else()
  message(WARNING "OpenSSL can not be found, test-rand_openssl will not be built")
endif()
#rngtest(dev_random) # disabled, because tests may go very long, pool may lack entropy
rngtest(dev_urandom)
