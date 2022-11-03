#!/bin/bash

mkdir -p dir-pics2

i=10
imax=18 # orig
imax=20
move=5
while [ ${i} -lt ${imax} ] ; do
    root -l -q -b drawMirror2.C+"($i,1,${move})"
    i=$((i+1))
done
