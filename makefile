my_ls : fun.o main.o
	gcc main.o fun.o -o my_ls
	make clean
main.o : ls.h main.c
	gcc -c main.c -o main.o
fun.o:ls.h fun.c
	gcc -c fun.c -o fun.o

.PHONY: clean
clean :
	rm  main.o fun.o
