add_subdirectory(basic)
add_subdirectory(common)
add_subdirectory(chip)
add_subdirectory(liberty)
add_subdirectory(power)
add_subdirectory(package)
add_subdirectory(parasitic)
add_subdirectory(verilog)
add_subdirectory(archive)

add_library(nano_db SHARED NanoDB.cpp)

target_link_libraries(nano_db
    nano_basic
    nano_common
    nano_chip
    nano_liberty
    nano_power
    nano_package
    nano_parasitic
    nano_verilog
    nano_archive
    ${MALLOC_LIB}
    ${PNG_LIBRARY} 
    Threads::Threads
    Boost::serialization
    dl
)