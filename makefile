interpretor:
	gcc -c argTable.c
	gcc -c shell.c
	gcc argTable.o shell.o -o shell

interpretor-dbg:
	gcc -g -c argTable.c
	gcc -g -c shell.c
	gcc -g argTable.o shell.o -o shell
