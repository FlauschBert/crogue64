# Crogue64 - A Roguelike for the Commodore 64 (WIP)

## Compilation Setup with CMake and LLVM-MOS SDK

Check https://github.com/llvm-mos/llvm-mos-sdk for downloading the SDK and extract the SDK package.  
Provide the path to CMake with `-DCMAKE_PREFIX_PATH=/path/to/llvm-mos-sdk` (or set the path to the VSCode CMake extension settings: `cmake.additionalCompilerSearchDirs`).  
You also have to specify the `-DCMAKE_TOOLCHAIN_FILE=/path/to/llvm-mos-sdk/lib/cmake/llvm-mos-sdk/llvm-mos-toolchain.cmake` for the linker settings.  
The `CMakePresets.json` file is already set up for this. You can use the presets with `-DCMAKE_PRESET=c64-debug` or `-DCMAKE_PRESET=c64-release`.

## Using VSCode with the VS64 Extension

The VS64 extension renders both the `CMakePresets.json` and `CMakeLists.txt` files obsolete.  
Only the `project-config.json` file is needed to configure the project.  
Checkout the project description at https://github.com/rolandshacks/vs64?tab=readme-ov-file.
