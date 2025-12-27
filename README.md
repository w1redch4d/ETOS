# ETOS (Evolved Technology Operating System)
ETOS is an operating system based on the Windows NT architecture, produced through clean-room reverse-engineering of official binaries. ETOS is not based on any existing source code, so contributors must use reverse-engineering software to replicate and improve upon the functionality found in official Windows binaries distributed by Microsoft.

## Supported Devices
Any x64 device with EFI firmware. i386 processors will be supported in the future.

## Building
The ETOS build system uses CMake. To generate the whole project's build files, run `cmake -S . -B build -DTARGET_ARCH=x64 -DTARGET_FIRMWARE=efi` from the root directory. This will generate the Makefiles (Linux) or The visual studio solutions (windows) in the `build` directory, to build the project run `cmake --build build`, this will generate the binaries in the `build` folders and its subfolders based on the project hierarchy.

## Running
To run ETOS, copy `${BUILDDIR}/bootmgr/bootmgfw.efi` to `/EFI/Microsoft/Boot/bootmgfw.efi` on an EFI system partition or execute `cmake --build build --target run` to run ETOS in the QEMU emulator. Note that to run in QEMU, you must have built or downloaded an EDKII OVMF firmware binary.

## Common Terms and Abbreviations
**i386**: The 32-bit x86 architecture, also known as IA-32. Includes the Intel i386 and all its 32-bit successors.

**x64**: The 64-bit x86 architecture, also known as x86-64, AMD64, Intel 64, EM64T, or IA-32e.

## Disclaimer
This project has been produced through a process known as clean-room reverse-engineering. To the best of the project maintainer's knowledge, no leaked, confidential, or illegally obtained Microsoft code or data was used or examined in the production of the code and data contained within this project. All contributors to this project must not obtain or use any knowledge of Microsoft confidential code or data.

## Copyright & Licensing
ETOS is provided under the BSD 3-Clause license, located in [LICENSE](LICENSE).
