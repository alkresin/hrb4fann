@echo off
set path=c:\softools\qt\tools\mingw482_32\bin
set HRB_DIR=%HB_PATH%
set HRB_LIBS=-lhbvm -lhbrdd -lhbmacro -lhbpp -lhbrtl -lhbcpage -lhblang -lhbcommon -lrddntx  -lrddcdx -lrddfpt -lhbsix -lgtgui -lgtwin -lhbcplr

%HRB_DIR%\bin\harbour %1.prg -n -i%HRB_DIR%\include;..\include %2

gcc -I. -I%HRB_DIR%\include -Wall -c %1.c -o%1.o
gcc -Wall -o%1.exe %1.o -L%HRB_DIR%\lib\win\mingw -L..\lib -Wl,--allow-multiple-definition -Wl,--start-group %HRB_LIBS% -lfann_double -lhrb4fann  -Wl,--end-group

del *.c
del *.o
