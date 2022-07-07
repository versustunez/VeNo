# Getting Started

You will need the following installed:
- Node.js
- CMake

The procedure to build is as follows:

1. (Optional) Generate CPP files for the UI based on configuration using one of the following methods. You will need to go through these every time you change the configuration files:
  - `pre-run.sh`
  - manual generation with Node.js by running `helper/parameter/parameterToCPP.js` and `helper/guiFiles/generate.js`
2. Build project using CMake. It is recommended to create and set the build directory prior to running CMake. You may do so by running the following command at the root of the repository:
  - `mkdir build && cd build && cmake .. && cmake --build .`