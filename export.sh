#!/bin/bash
mkdir -p .temp && cd .temp
# loop
cp -r ../songs/createOW .
cd createOW
mv create.ogg.dat EasyStandard.dat
mv create.ogg create.egg
zip ../../exports/createOW.zip *
cd - > /dev/null 2>&1

cp -r ../songs/blueSky .
cd blueSky
mv song.ogg.dat Normal.dat
mv song.ogg song.egg
zip ../../exports/blueSky.zip *
cd - > /dev/null 2>&1

cp -r ../songs/testAudio .
cd testAudio
mv testAudio.ogg_notation.json.dat Normal.dat
mv testAudio.ogg testAudio.egg
zip ../../exports/testAudio.zip *
cd - > /dev/null 2>&1

# \loop
cd ..
rm -r .temp