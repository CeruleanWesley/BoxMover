#!/bin/bash

if [ -z $1 ]; then
  echo 'usage: rebuild_box_mover.sh {root_path_of_project}'
  exit 0
fi

build_dir=$1"/build"
rm -rf $build_dir
cmake -B $build_dir
cmake --build $build_dir
