# nmake makefile
#
# Tools used:
#  Compile::Watcom Resource Compiler
#  Compile::GNU C
#  Make: nmake or GNU make
all : SETOPT.exe

SETOPT.exe : SETOPT.obj SETOPT.res SETOPT.def
	gcc -Zomf SETOPT.obj SETOPT.res SETOPT.def -o SETOPT.exe
	rc SETOPT.res

SETOPT.obj : SETOPT.c SETOPT.h SETCOLOR.H SETFONT.H
	gcc -Wall -Zomf -O2 -c SETOPT.c -o SETOPT.obj

SETOPT.res : SETOPT.rc 
	rc -r SETOPT.rc

clean :
	rm -rf *exe *RES *obj
