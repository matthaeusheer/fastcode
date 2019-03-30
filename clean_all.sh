#!/usr/bin/env bash

declare -a arr=("penguin" "HGWOSCA" "tests" "benchmark");

for d in "${arr[@]}"
do
    echo "--- Cleaning build files in $d ---"
    (cd "$d" && make clean)
    echo " "
done