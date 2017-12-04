include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(tosha3)

macro(edtest name sign_input)
    addtest(test-${name} ed25519_test.cpp)
    target_compile_definitions(test-${name} PUBLIC
        -DTESTDATA_PATH=${sign_input}
        )
    target_link_libraries(test-${name}
        ${lib}
        ${ARGN}
        )
endmacro()


if(AMD64_OPTIMIZED)
    if(OPENSSL_FOUND)
        edtest(amd64-64-24k-sha2
            ${CMAKE_CURRENT_SOURCE_DIR}/sign.input.with.sha2.txt
            # link with
            amd64-64-24k
            sha2_openssl
            dev_urandom
    	)
    endif()

    edtest(amd64-64-24k-sha3
        ${CMAKE_CURRENT_SOURCE_DIR}/sign.input.with.sha3.txt

        # link with
        amd64-64-24k
        sha3_brainhub
        dev_urandom
        )
endif()



if(OPENSSL_FOUND)
    edtest(ref10-sha2
        ${CMAKE_CURRENT_SOURCE_DIR}/sign.input.with.sha2.txt

        # link with
        ref10
        sha2_openssl
        rand_openssl
    )
endif()

edtest(ref10-sha3
    ${CMAKE_CURRENT_SOURCE_DIR}/sign.input.with.sha3.txt

    # link with
    ref10
    sha3_brainhub
    dev_urandom
    )
