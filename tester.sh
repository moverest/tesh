#!/bin/bash
echo Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. > tmp-lorem

num_successes=0
num_failures=0

for f in tests/*_in; do
  test_path=${f/_in/}
  if diff --color <(./tesh < $f) ${test_path}_out
  then
    echo -e "\x1b[32m[PASSED]\x1b[0m $test_path"
    num_successes=`expr $num_successes + 1`
  else
      echo -e "\x1b[31m[FAILED]\x1b[0m $test_path"
      num_failures=`expr $num_failures + 1`
  fi
done

echo
echo -e "\x1b[32m$num_successes succeeded.\x1b[0m"
echo -e "\x1b[31m$num_failures failed.\x1b[0m"

rm tmp*

[ $num_failures == 0 ]
