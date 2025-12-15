add_library(nanoParasitic
    NSParasitic.cpp
    NSParasiticNet.cpp
)

target_link_libraries(nanoParasitic
    nanoBasic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)