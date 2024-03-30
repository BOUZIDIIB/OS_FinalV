LIB := -Iinclude

all : list op lib menu main

op : src/object/operation.c 
	gcc -c $^ $(LIB) -o bin/op.o

list : src/object/linkedList.c 
	gcc -c $^ $(LIB) -o bin/list.o

lib : src/object/libOS.c 
	gcc -c $^ $(LIB) -o bin/lib.o

menu : src/object/menu.c 
	gcc -c $^ $(LIB) -o bin/menu.o

main : src/main.c bin/lib.o bin/op.o bin/list.o bin/menu.o
	gcc $^ $(LIB) -o bin/main

exe : 
	bin/main

clean :
	rm -f bin/*
