add_library(sha3_brainhub
    sha3.c
    sha3_512.c
    )
set_target_properties(sha3_brainhub PROPERTIES EXCLUDE_FROM_ALL TRUE)
