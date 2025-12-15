add_library(nano_basic
    NSDatabase.cpp
)

target_link_libraries(nano_basic
    nano_common
    nano_chip
    nano_liberty
    nano_power
    nano_package
    nano_parasitic
    nano_verilog
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)