add_library(nano_verilog
    NSVerilog.cpp
)

target_link_libraries(nano_verilog
    nano_basic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)