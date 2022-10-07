#!/bin/bash

if [ "$1" = "-1" ];then
    ./atpg -path -start G3 -end PO_G16 ../../circuits/iscas85/c17.bench 
    echo "---------------------"
    echo "Correct ans of c17: 2"
    ./atpg -path -start G3 -end PO_G16 ../../circuits/iscas85/c17.bench | grep "Testcase result:"
fi

if [ "$1" = "-2" ];then
    ./atpg -path -start 126GAT_30 -end PO_863GAT_424 ../../circuits/iscas85/c880.bench 
    echo "---------------------"
    echo "Correct ans of c880: 4"
    ./atpg -path -start 126GAT_30 -end PO_863GAT_424 ../../circuits/iscas85/c880.bench | grep "Testcase result:"
fi

if [ "$1" = "-3" ];then
    ./atpg -path -start 307GAT_18 -end PO_2548GAT_840 ../../circuits/iscas85/c6288.bench 
    echo "---------------------"
    echo "Correct ans of c6288: 468"
    ./atpg -path -start 307GAT_18 -end PO_2548GAT_840 ../../circuits/iscas85/c6288.bench | grep "Testcase result:"
fi
