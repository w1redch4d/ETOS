cmake_minimum_required(VERSION 3.21)

project(ETOS LANGUAGES C ASM)


set(TARGET_FAMILY "x86" CACHE STRING "Target family")
set(TARGET_ARCH "x64" CACHE STRING "Target architecture (x64 or i386)")
set(TARGET_FIRMWARE "efi" CACHE STRING "Target firmware")

set(BUILDDIR "${CMAKE_BINARY_DIR}" CACHE PATH "Build directory")

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_AR /usr/bin/llvm-ar)


add_compile_options(
    -Wall
    -Wextra
    -ffreestanding
    -fno-stack-protector
)

# Architecture-specific configuration
if (TARGET_ARCH STREQUAL "x64")
    add_compile_options(
        -target x86_64-pc-win32
        -mno-red-zone
    )
    set(QEMU qemu-system-x86_64)

    if (TARGET_FIRMWARE STREQUAL "efi")
        set(QEMU_BIOS RELEASEX64_OVMF.fd)
        set(OVMF_PATH /usr/share/OVMF/x64/OVMF.4m.fd)
        set(BOOTMGFW_NAME bootx64.efi)
    endif()

elseif (TARGET_ARCH STREQUAL "i386")
    add_compile_options(
        -target i386-pc-win32
    )
    set(QEMU qemu-system-i386)

    if (TARGET_FIRMWARE STREQUAL "efi")
        set(QEMU_BIOS RELEASEIa32_OVMF.fd)
        set(BOOTMGFW_NAME bootia32.efi)
    endif()
endif()

set(QEMUFLAGS --enable-kvm)

if (TARGET_FIRMWARE STREQUAL "efi")
    list(APPEND QEMUFLAGS
        -bios ${QEMU_BIOS}
        -hda fat:rw:disk
    )
endif()


add_subdirectory(sdk/crt)
add_subdirectory(sdk/rtl)
add_subdirectory(boot)

if (TARGET_FIRMWARE STREQUAL "efi")
    add_custom_target(run
        COMMAND ${CMAKE_COMMAND} -E make_directory disk/EFI/Boot
        COMMAND ${CMAKE_COMMAND} -E copy
            ${BUILDDIR}/bootmgr/bootmgfw.efi
            disk/EFI/Boot/${BOOTMGFW_NAME}
        COMMAND $(CMAKE_COMMAND) -E copy ${OVMF_PATH} ${QEMU_BIOS}
        COMMAND ${QEMU} ${QEMUFLAGS}
        DEPENDS bootmgr
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
else()
    add_custom_target(run
        COMMAND ${QEMU} ${QEMUFLAGS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
