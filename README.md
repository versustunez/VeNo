[![Build](https://github.com/versustunez/VeNo/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/versustunez/VeNo/actions/workflows/build.yml)

<img src="./Plugin/Assets/Logo.svg" alt="VeNo Logo" style="margin: auto; display: block;">

# VeNo


![Screenshot](./Assets/Screenshot1.png)

VeNo started as a pure learning project for C ++ and the understanding of how synths and audio processing work.

The learning project turned into the dream of a new open source synthesizer which offers enough features for newcomers

## Features

- 4 Oscillators
- 2 LFOs
- 2 Random Generator
- Dual Filter
- Distortion
- Compressor
- GUI
    - Layout-able GUI via .vui files (or let them default :P)
    - Colorable GUI
  
## GUI-Lang

VeNo use a custom-written Layout language to optimize the relayouting for me

Why self written? because of learning perspective and not happy with pre-solutions

For information look into: DOCS/GUILang.md

## Build

For building you need:
- CMake `3.16 or Newer`
- Compiler (MSVC, GCC, Clang)
- Visual Studio (Windows only)
- Terminal :P

### Generic
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE={Release|Debug}
cmake --build .
```


