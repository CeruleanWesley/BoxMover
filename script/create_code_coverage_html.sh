#!/bin/bash

if [ -z $1 ]; then
  echo 'usage: create_code_coverage_html.sh {root_path_of_project}'
  exit 0
fi

root_path=$1
./$root_path/script/rebuild_box_mover.sh $root_path

build_path=$root_path/build
./$build_path/test/game_test
lcov -c  -d $build_path/src/CMakeFiles/BoxMoverLib.dir/ -o $build_path/test.info
lcov -r $build_path/test.info "/usr/*" -o $build_path/test.info
genhtml $build_path/test.info --output-directory $build_path/out
firefox $build_path/out/index.html
