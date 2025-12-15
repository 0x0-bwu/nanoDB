add_library(nano_parasitic
    NSParasitic.cpp
    NSParasiticNet.cpp
)

target_link_libraries(nano_parasitic
    nano_basic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)