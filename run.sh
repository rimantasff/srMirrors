#!/bin/bash

i=0
imax=18 # orig
#imax=8
move=0
while [ ${i} -lt ${imax} ] ; do
    root -l -q -b drawMirror.C+"($i,1,${move})"
    i=$((i+1))
done
