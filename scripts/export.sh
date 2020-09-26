#!/bin/bash
if [ "${PWD##*/}" == "scripts" ]; then
    cd ..
fi
mkdir -p .temp 
cd .temp

for src in ../songs/* ; do

    dest="${src##*/}"
    echo "$dest"
    mkdir -p "$dest"
    cp -r "$src" .
    for m in "$dest"/*.ogg; do
        mv -- "$m" "${m%.ogg}.egg"
    done

    cd "$dest"
    zip ../../exports/"$dest.zip" *
    cd ..
done

cd ..
rm -r .temp