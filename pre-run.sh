#!/usr/bin/bash

if ! [ -x "$(command -v node)" ]; then
  echo -e "NODE NOT INSTALLED";
  exit 1;
fi

echo -e "NODEJS FOUND!";
echo -e "START PRE-BUILD";
cd helper || exit
# RUN our both generation helper
cd parameter || exit
node ./parameterToCPP.js
cd ../guiFiles || exit
node ./generate.js

cd ../../Plugin || exit
clang-format -i Source/**/*.cpp | exit
clang-format -i Source/**/*.h | exit

echo -e "END PRE-BUILD";
