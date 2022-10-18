#!/bin/bash

path=./mod_logsim_IO

# For the first part
./atpg -logicsim -input $path/input/c17.input ../circuits/iscas85/c17.bench | sed -n '4,+7p' > tmp/tmpA && cat $path/output/c17.output | awk -F' ' '{print $2 " " $4}' > tmp/tmpB && diff tmp/tmpA tmp/tmpB && echo "c17.bench Ok"

./atpg -logicsim -input $path/input/c7552.input ../circuits/iscas85/c7552.bench | sed -n '4,+39p' > tmp/tmpA && cat $path/output/c7552.output | awk -F' ' '{print $2 " " $4}' > tmp/tmpB && diff tmp/tmpA tmp/tmpB && echo "c7522.bench Ok"

# For the second part check logicsim and modified logicsim

for f in $path/input/c17*;
do
    echo "-logicsim"
    /usr/bin/time -f "Average memory usage: %K \nMaximum memory usage: %M\n" ./atpg -logicsim -input $f ../circuits/iscas85/c17.bench > tmp/tmpA
    grep CPU tmp/tmpA

    echo "-mod_logicsim"
    /usr/bin/time -f "Average memory usage: %K \nMaximum memory usage: %M\n" ./atpg -mod_logicsim -input $f ../circuits/iscas85/c17.bench > tmp/tmpB
    grep CPU tmp/tmpB

    diff tmp/tmpA tmp/tmpB | grep -v "c\|Run\|CP\|---" || echo "$f Ok"
    echo "---"
done

for f in $path/input/c7552*;
do
    echo "-logicsim"
    /usr/bin/time -f "Average memory usage: %K \nMaximum memory usage: %M\n" ./atpg -logicsim -input $f ../circuits/iscas85/c7552.bench > tmp/tmpA
    grep CPU tmp/tmpA

    echo "-mod_logicsim"
    /usr/bin/time -f "Average memory usage: %K \nMaximum memory usage: %M\n" ./atpg -mod_logicsim -input $f ../circuits/iscas85/c7552.bench > tmp/tmpB
    grep CPU tmp/tmpB

    diff tmp/tmpA tmp/tmpB | grep -v "c\|Run\|CP\|---" || echo "$f Ok"
    echo "---"
done
