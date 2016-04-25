#!/bin/bash
export FANN_DIR=../../FANN_220

if ! [ -e ../lib ]; then
  mkdir ../lib
  chmod a+w+r+x ../lib
fi

gcc -I$FANN_DIR/src/include -Wall -c $FANN_DIR/src/doublefann.c -odoublefann.o
rm ../lib/libfann_double.a
ar rc ../lib/libfann_double.a doublefann.o
rm *.o