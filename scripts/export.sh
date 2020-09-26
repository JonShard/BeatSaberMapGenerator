#!/bin/bash
if [ "${PWD##*/}" == "scripts" ]; then
    cd ..
fi

mkdir -p .temp && cd .temp
# loop
cp -r ../songs/createOW .
cd createOW
mv create.dat EasyStandard.dat
mv create.ogg create.egg
zip ../../exports/createOW.zip *
cd - > /dev/null 2>&1

cp -r ../songs/blueSky .
cd blueSky
mv song.dat Normal.dat
mv song.ogg song.egg
zip ../../exports/blueSky.zip *
cd - > /dev/null 2>&1

cp -r ../songs/testAudio .
cd testAudio
mv testAudio.dat Normal.dat
mv testAudio.ogg testAudio.egg
zip ../../exports/testAudio.zip *
cd - > /dev/null 2>&1

cp -r ../songs/DiscoDescentCryptOfTheNecrodancer .
cd DiscoDescentCryptOfTheNecrodancer
mv song.dat ExpertPlusStandard.dat
mv song.ogg song.egg
zip ../../exports/discoDescent.zip *
cd - > /dev/null 2>&1

# \loop
cd ..
rm -r .temp