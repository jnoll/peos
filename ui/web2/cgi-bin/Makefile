all:
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o events.o vm.o process_table.o process.o action.o
	chmod og+rx main.cgi
main:
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o events.o vm.o process_table.o process.o action.o
	chmod og+rx main.cgi

