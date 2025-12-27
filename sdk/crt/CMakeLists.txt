cmake_minimum_required(VERSION 3.21)

project(crt)

set(CRT_SOURCES
    stdio/wprintf.c
    string/mem.c
    string/str.c
    string/wmem.c
    string/wstr.c
)

add_library(crt STATIC ${CRT_SOURCES})

set_target_properties(crt PROPERTIES
    OUTPUT_NAME crt
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/crt
)


target_include_directories(crt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/crt
)

target_compile_options(crt PRIVATE
    -ffreestanding
)
