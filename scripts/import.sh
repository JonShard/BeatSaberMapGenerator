#!/bin/bash
if [ "${PWD##*/}" == "scripts" ]; then
    cd ..
fi
mkdir -p songs

for f in imports/*.zip; do
    
    name="${f##*/}"
    name=${name%.zip*}
    name="$(echo $name | cut -d "(" -f2 | cut -d ")" -f1)"
    name="songs/$name"
    echo $name

    mkdir -p "$name"
    unzip -n "$f" -d "$name"
    for m in "$name"/*.egg; do
        mv -- "$m" "${m%.egg}.ogg"
    done
done