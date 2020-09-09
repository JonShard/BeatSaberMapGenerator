#!/bin/bash
mkdir -p .temp && cd .temp
# loop
cp -r ../songs/createOW .
cd createOW
mv create.ogg.dat EasyStandard.dat
mv create.ogg create.egg
zip ../../exports/createOW.zip *
cd - > /dev/null 2>&1
# \loop
cd ..
rm -r .temp