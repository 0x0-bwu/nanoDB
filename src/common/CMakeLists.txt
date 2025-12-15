add_library(nanoCommon
    NSLookupTable.cpp
    NSMaterial.cpp
    NSShape.cpp
)

target_link_libraries(nanoCommon
    nanoBasic
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)