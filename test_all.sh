#!/usr/bin/env bash

declare -a arr=("penguin");

for d in "${arr[@]}"
do
    (cd "$d" && make)
done

cd tests;
make;
make test;
