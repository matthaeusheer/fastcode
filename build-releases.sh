#!/usr/bin/env bash

# This is a shell script to create executables for different releases in a folder called build-releases.
# The executables will be named benchmark_<version-tag>

##################################### CAREFUL: ###################################################
# This is a very rough script for convenience, if the git checkout fails (e.g. unknown release) it keeps running
# on the current branch and might compile the same release executable all over again.
# The checkout also does not work if you have un-commited changes on your branch.
##################################################################################################

build_releases_dir="build-releases"
declare -a  tags=("v0.0.1" "pso0.0.1" "pso0.0.2" "pso0.0.3" "pso0.0.4" "pso0.0.5" "pso0.0.6")

original_branch=$(git branch | sed -n -e 's/^\* \(.*\)/\1/p')

if [ -d "$build_releases_dir" ]; then rm -Rf $build_releases_dir; fi
mkdir $build_releases_dir
cd $build_releases_dir

for tag in "${tags[@]}"
do
   echo "$tag"
   git checkout $tag
   cmake ..; make benchmark
   mv benchmark benchmark_$tag
done

echo "Go back to original branch: $original_branch"
git checkout $original_branch

cd ..

