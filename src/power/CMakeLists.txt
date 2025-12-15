add_library(nano_power
    NSLossPower.cpp
)

target_link_libraries(nano_power
    nano_basic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)