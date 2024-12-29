add_subdirectory(basic)
add_subdirectory(chip)
add_subdirectory(liberty)
add_subdirectory(parasitic)
add_subdirectory(verilog)

add_library(nanoDB SHARED NanoDB.cpp)

target_link_libraries(nanoDB
    nanoBasic
    nanoChip
    nanoLiberty
    nanoParasitic
    nanoVerilog
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    boost_serialization
    dl
)