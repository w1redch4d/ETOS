cmake_minimum_required(VERSION 3.21)

project(rtl)


set(RTL_SOURCES
    guid.c
    string.c
)

add_library(rtl STATIC ${RTL_SOURCES})

# Ensure the output name is exactly "rtl.lib"
set_target_properties(rtl PROPERTIES
    OUTPUT_NAME rtl
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/rtl
)

target_include_directories(rtl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/crt
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/nt
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/rtl
)

target_compile_options(rtl PRIVATE
    -ffreestanding
)