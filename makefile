interpretor:
	gcc -c argTable.c
	gcc -c shell.c
	gcc argTable.o shell.o -o shell

interpretor-dbg:
	gcc -g -c argTable.c
	gcc -g -c shell.c
	gcc -g argTable.o shell.o -o shell

myls-dbg:
	gcc -g -c Queue.c
	gcc -g -c myls.c
	gcc -g myls.o Queue.o -o myls
