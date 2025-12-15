add_library(nanoChip
    NSChip.cpp
    NSChipBlock.cpp
    NSChipBTerm.cpp
    NSChipInst.cpp
    NSChipITerm.cpp
    NSChipNet.cpp
)

target_link_libraries(nanoChip
    nanoBasic
    nanoCommon
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)