path = ../../../os/kernel/

all:
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o $(path)events.o $(path)vm.o $(path)process_table.o $(path)process.o $(path)action.o
	chmod og+rx main.cgi
main:
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o $(path)events.o $(path)vm.o $(path)process_table.o $(path)process.o $(path)action.o
	chmod og+rx main.cgi

