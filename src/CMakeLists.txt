add_subdirectory(basic)
add_subdirectory(common)
add_subdirectory(chip)
add_subdirectory(liberty)
add_subdirectory(power)
add_subdirectory(package)
add_subdirectory(parasitic)
add_subdirectory(verilog)
add_subdirectory(archive)

add_library(nanoDB SHARED NanoDB.cpp)

target_link_libraries(nanoDB
    nanoBasic
    nanoCommon
    nanoChip
    nanoLiberty
    nanoPower
    nanoPackage
    nanoParasitic
    nanoVerilog
    nanoArchive
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)