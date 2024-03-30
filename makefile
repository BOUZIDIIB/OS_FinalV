LIB := -Iinclude
CFLAGS = -Wall -Werror -I/usr/include


all : list op lib main projet

op : src/operation.c 
	gcc -c $^ $(LIB) -o bin/op.o

list : src/linkedList.c 
	gcc -c $^ $(LIB) -o bin/list.o

lib : src/libOS.c bin/op.o
	gcc -c $< $(LIB) -o bin/lib.o


main : src/test.c bin/op.o bin/list.o
	gcc $^ $(LIB) -o bin/main

projet : src/testLib.c bin/lib.o bin/op.o bin/list.o
	gcc $^ $(LIB) -o bin/projet

















inter : src/testInterface.c
	gcc $^ -o bin/inter -lncurses -largp

exe : main
	bin/main

exe_projet : projet
	bin/projet

exe_inter :
	bin/inter












clean :
	rm bin/*