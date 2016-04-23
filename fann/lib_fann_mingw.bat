@echo off

set FANN_DIR=..\..\FANN_220
set path=c:\softools\qt\tools\mingw482_32\bin

if not exist ..\lib md ..\lib

gcc -I%FANN_DIR%\src\include -Wall -c %FANN_DIR%\src\doublefann.c -odoublefann.o
del ..\lib\libfann_double.a
ar rc ..\lib\libfann_double.a doublefann.o
del *.o

