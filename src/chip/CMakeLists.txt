add_library(nano_chip
    NSChip.cpp
    NSChipBlock.cpp
    NSChipBTerm.cpp
    NSChipInst.cpp
    NSChipITerm.cpp
    NSChipNet.cpp
)

target_link_libraries(nano_chip
    nano_basic
    nano_common
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)