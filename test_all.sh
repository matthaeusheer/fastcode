#!/usr/bin/env bash

declare -a arr=("penguin" "HGWOSCA" "bench");

for d in "${arr[@]}"
do
    (cd "$d" && make clean && make)
done

for d in "tests" "bench";
do
    (cd "$d" && make clean && make && make test)
done
