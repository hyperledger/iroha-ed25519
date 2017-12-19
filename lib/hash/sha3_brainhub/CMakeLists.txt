set(LIB sha3_brainhub)
add_library(${LIB} OBJECT
    sha3.c
    sha3_256.c
    sha3_512.c
    )
set_target_properties(${LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)
