cmake_minimum_required(VERSION 3.21)

project(boot LANGUAGES C ASM)

set(APP "bootmgr" CACHE STRING "Boot application")

set(BOOT_SOURCES
    lib/io/fs/fat.c
    lib/io/device.c
    lib/io/file.c
    lib/io/io.c

    lib/misc/event.c
    lib/misc/option.c
    lib/misc/string.c

    lib/mm/mm.c
    lib/mm/mmha.c
    lib/mm/mmmd.c
    lib/mm/mmpa.c

    lib/bootlib.c
)


if (APP STREQUAL "bootmgr")
    list(APPEND BOOT_SOURCES
        app/bootmgr/bcd.c
        app/bootmgr/bootmgr.c
        app/bootmgr/fw.c
    )
endif()

if (TARGET_ARCH STREQUAL "x64")
    list(APPEND BOOT_SOURCES
        lib/x64/arch.c
        lib/x64/archasm.S
    )
endif()

if (TARGET_FIRMWARE STREQUAL "efi")
    list(APPEND BOOT_SOURCES
        lib/efi/eficon.c
        lib/efi/efidebug.c
        lib/efi/efierr.c
        lib/efi/efifw.c
        lib/efi/efiinit.c
        lib/efi/efiwrap.c
    )

    if (APP STREQUAL "bootmgr")
        list(APPEND BOOT_SOURCES
            app/bootmgr/efi/efientry.c
        )
    endif()
endif()

add_executable(${APP} ${BOOT_SOURCES})

if (TARGET_FIRMWARE STREQUAL "efi" AND APP STREQUAL "bootmgr")
    set_target_properties(${APP} PROPERTIES
        OUTPUT_NAME bootmgfw
        SUFFIX ".efi"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bootmgr
    )
endif()

target_include_directories(${APP} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/inc/nt
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/inc/crt
)

if (TARGET_FIRMWARE STREQUAL "efi")
    target_include_directories(${APP} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/inc/efi
    )

    target_compile_definitions(${APP} PRIVATE
        _EFI
    )
endif()

target_link_libraries(${APP} PRIVATE
    crt
    rtl
)

if (TARGET_FIRMWARE STREQUAL "efi")
    target_link_options(${APP} PRIVATE
       -target x86_64-unknown-windows
       -nostdlib
       -Wl,-entry:EfiEntry
       -Wl,-subsystem:EFI_APPLICATION
       -fuse-ld=lld
    )
endif()
