#!/usr/bin/sh

cd helper || exit
# RUN our both generation helper
cd parameter || exit
node ./parameterToCPP.js
cd ../guiFiles || exit
node ./generate.js