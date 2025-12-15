add_library(nano_common
    NSLookupTable.cpp
    NSMaterial.cpp
    NSShape.cpp
)

target_link_libraries(nano_common
    nano_basic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)