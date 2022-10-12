#!/bin/bash
./atpg -logicsim -input ./input/c17.input ../circuits/iscas85/c17.bench | sed -n '4,+7p' > tmp/tmpA && cat output/c17.output | awk -F' ' '{print $2 " " $4}' > tmp/tmpB && diff tmp/tmpA tmp/tmpB && echo "c17.bench Ok"

./atpg -logicsim -input ./input/c7552.input ../circuits/iscas85/c7552.bench | sed -n '4,+39p' > tmp/tmpA && cat output/c7552.output | awk -F' ' '{print $2 " " $4}' > tmp/tmpB && diff tmp/tmpA tmp/tmpB && echo "c7522.bench Ok"
