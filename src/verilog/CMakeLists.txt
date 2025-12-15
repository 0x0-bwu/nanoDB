add_library(nanoVerilog
    NSVerilog.cpp
)

target_link_libraries(nanoVerilog
    nanoBasic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)