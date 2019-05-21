#!/usr/bin/env bash

# This is a shell script to create executables for different releases in a folder called build-releases.
# The executables will be named benchmark_<version-tag>

declare -a  tags=("v0.0.1" "v0.0.2" "v0.0.3" "v0.0.4" "v0.0.5" "v0.0.6")

original_branch=$(git branch | sed -n -e 's/^\* \(.*\)/\1/p')

mkdir build-releases
cd build-releases

for tag in "${tags[@]}"
do
   echo "$tag"
   git checkout $tag
   cmake ..; make benchmark
   mv benchmark benchmark_$tag
done

echo "Go back to original branch: $original_branch"
git checkout $original_branch

