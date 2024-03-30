LIB := -Iinclude

all : op list main

op : src/operation.c 
	gcc -c $^ $(LIB) -o bin/$@.o

list : src/linkedList.c 
	gcc -c $^ $(LIB) -o bin/$@.o


main : src/test.c bin/op.o bin/list.o
	gcc $^ $(LIB) -o bin/main

exe :
	bin/main