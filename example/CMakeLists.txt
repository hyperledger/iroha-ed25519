cmake_minimum_required(VERSION 3.0)
project(ed25519_example C)

find_package(ed25519 REQUIRED)

add_executable(test1
    main.c
    )
target_link_libraries(test1
    ed25519
    )
