#!/bin/bash
export HRB_DIR=../../../harbour
export FANN_DIR=../FANN_220

if ! [ -e ../lib ]; then
  mkdir ../lib
  chmod a+w+r+x ../lib
fi

gcc -I$FANN_DIR/src/include -I$HRB_DIR/include -Wall -c source/fann2hrb.c -ofann2hrb.o
rm lib/libhrb4fann.a
ar rc lib/libhrb4fann.a fann2hrb.o
rm *.o
