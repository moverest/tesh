#!/bin/bash
echo Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. > tmp-lorem
for i in tests/*_in; do
  ./tesh < $i > tmp
  DIFFF=`diff tmp ${i/_in/_out}`
  if [ -z "${DIFFF}" ]; then
    echo "Test ${i/_in/} OK"
  elif [ -n "${DIFFF}" ]; then
    echo "Test ${i/_in/} FAILED"
  fi
done

rm tmp*
