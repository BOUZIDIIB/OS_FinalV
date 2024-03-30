LIB := -Iinclude
CFLAGS = -Wall -Werror -I/usr/include


all : list op lib main

op : src/object/operation.c 
	gcc -c $^ $(LIB) -o bin/op.o

list : src/object/linkedList.c 
	gcc -c $^ $(LIB) -o bin/list.o

lib : src/object/libOS.c bin/op.o
	gcc -c $< $(LIB) -o bin/lib.o




main : src/testLib.c bin/lib.o bin/op.o bin/list.o
	gcc $^ $(LIB) -o bin/main

exe : 
	bin/main


clean :
	rm bin/*