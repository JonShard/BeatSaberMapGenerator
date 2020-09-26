mkdir -p extractedMaps

i=0
for f in imports/*.zip; do 
    unzip "$f" -d extractedMaps
    rm extractedMaps/*.jpg &>/dev/null
    rm extractedMaps/*.jpeg &>/dev/null
    rm extractedMaps/*.png &>/dev/null
    rm extractedMaps/*.egg &>/dev/null
    rm extractedMaps/info.dat &>/dev/null
    rm extractedMaps/Info.dat &>/dev/null
    for m in extractedMaps/*.dat; do
        mv "$m" extractedMaps/"map$i.dat"
        let "i+=1"
    done
done