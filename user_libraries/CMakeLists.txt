
add_library(user_libraries INTERFACE)

target_include_directories(user_libraries
    INTERFACE
    .
    ../mbed-os
)

target_sources(user_libraries
    INTERFACE
    connectToROS.cpp
)