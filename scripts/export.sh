#!/bin/bash
if [ "${PWD##*/}" == "scripts" ]; then
    cd ..
fi
mkdir -p .temp 
cd .temp

for src in ../songs/* ; do

    dest="${src##*/}"
    mkdir -p "$dest"
    cp -r "$src" .
    for m in "$dest"/*.ogg; do
        mv -- "$m" "${m%.ogg}.egg" &>/dev/null
    done
    for d in "$dest"/*.dat; do
        if [[ "$d" == *"info.dat" ]] || [[ "$d" == *"Info.dat" ]] || [[ "$d" == *"song.dat" ]]; then
            continue
        fi
        printf "$dest/song.dat -> $d\n"
        mv "$dest"/song.dat "$d" &>/dev/null
    done
    cd "$dest"
    zip ../../exports/"$dest.zip" * &>/dev/null
    cd ..
done

cd ..
rm -r .temp

