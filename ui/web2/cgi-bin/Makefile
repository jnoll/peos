test:
	gcc -g -g -I.  -c -o test.o test.c
	gcc -o test.cgi test.o events.o vm.o process_table.o process.o action.o
create:
	gcc -g -g -I.  -c -o create.o create.c
	gcc -o create.cgi create.o events.o vm.o process_table.o process.o action.o
	chmod og+rx create.cgi
run:
	gcc -g -g -I.  -c -o run.o run.c
	gcc -o run.cgi run.o events.o vm.o process_table.o process.o action.o
run2:
	gcc -g -g -I.  -c -o run2.o run2.c
	gcc -o run2.cgi run2.o events.o vm.o process_table.o process.o action.o
main:
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o events.o vm.o process_table.o process.o action.o
	chmod og+rx main.cgi
action:
	gcc -g -g -I.  -c -o action_page.o action_page.c
	gcc -o action_page.cgi action_page.o events.o vm.o process_table.o process.o action.o
	chmod og+rx action_page.cgi
do:
	gcc -g -g -I.  -c -o do.o do.c
	gcc -o do.cgi do.o events.o vm.o process_table.o process.o action.o
	chmod og+rx do.cgi

