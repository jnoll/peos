all:
	gcc -g -g -I.  -c -o create.o create.c
	gcc -o create.cgi create.o events.o vm.o process_table.o process.o action.o
	chmod og+rx create.cgi
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o events.o vm.o process_table.o process.o action.o
	chmod og+rx main.cgi
	gcc -g -g -I.  -c -o action_page.o action_page.c
	gcc -o action_page.cgi action_page.o events.o vm.o process_table.o process.o action.o
	chmod og+rx action_page.cgi
	gcc -g -g -I.  -c -o do.o do.c
	gcc -o do.cgi do.o events.o vm.o process_table.o process.o action.o
	chmod og+rx do.cgi
	gcc -g -g -I.  -c -o do_create.o do_create.c
	gcc -o do_create.cgi do_create.o events.o vm.o process_table.o process.o action.o
	chmod og+rx do_create.cgi
create:
	gcc -g -g -I.  -c -o create.o create.c
	gcc -o create.cgi create.o events.o vm.o process_table.o process.o action.o
	chmod og+rx create.cgi
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
do_create:
	gcc -g -g -I.  -c -o do_create.o do_create.c
	gcc -o do_create.cgi do_create.o events.o vm.o process_table.o process.o action.o
	chmod og+rx do_create.cgi

