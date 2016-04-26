#!/bin/bash

export FANN_DIR=../../FANN_220
export HRB_DIR=../../../../harbour
export SYSTEM_LIBS="-lm"
export HARBOUR_LIBS="-lhbdebug -lhbvm -lhbrtl -lgttrm -lhblang -lhbrdd -lhbmacro -lhbpp -lrddntx -lrddcdx -lrddfpt -lhbsix -lhbcommon -lhbcpage"

$HRB_DIR/bin/linux/gcc/harbour $1 -n -q -i$HRB_DIR/include -i$FANN_DIR/src/include -w2
gcc $1.c -o$1 -I $HRB_DIR/include -L $HRB_DIR/lib/linux/gcc -L ../lib -Wl,--start-group $HARBOUR_LIBS -lfann_double -lhrb4fann -Wl,--end-group $SYSTEM_LIBS

rm *.c