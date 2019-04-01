#!/usr/bin/env bash

declare -a arr=("penguin" "HGWOSCA" "benchmark");

for d in "${arr[@]}"
do
    (cd "$d" && make clean && make)
done

for d in "tests" "benchmark";
do
    (cd "$d" && make clean && make && make test)
done
