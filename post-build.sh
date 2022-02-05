#!/usr/bin/bash

cd Plugin || exit;

clang-format -i Source/**/*.cpp | exit
clang-format -i headers/**/*.h | exit