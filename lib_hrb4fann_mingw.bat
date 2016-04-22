@echo off

set HRB_DIR=%HB_PATH%
set FANN_DIR=..\FANN_220
set path=c:\softools\qt\tools\mingw482_32\bin

if not exist lib md lib

gcc -I%FANN_DIR%\src\include -I%HRB_DIR%\include -Wall -c source\fann2hrb.c -ofann2hrb.o
del lib\libhrb4fann.a
ar rc lib\libhrb4fann.a fann2hrb.o
del *.o
