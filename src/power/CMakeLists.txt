add_library(nanoPower
    NSLossPower.cpp
)

target_link_libraries(nanoPower
    nanoBasic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)