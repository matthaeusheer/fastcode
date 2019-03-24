#!/usr/bin/env bash

declare -a arr=("penguin" "HGWOSCA");

for d in "${arr[@]}"
do
    (cd "$d" && make clean && make)
done

cd tests;
make clean;
make;
make test;
