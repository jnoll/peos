path=../../../os/kernel/

all:
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o $(path)events.o $(path)vm.o $(path)process_table.o $(path)process.o $(path)action.o
	chmod og+rx main.cgi
	gcc -g -g -I.  -c -o action_page.o action_page.c
	gcc -o action_page.cgi action_page.o $(path)events.o $(path)vm.o $(path)process_table.o $(path)process.o $(path)action.o
	chmod og+rx action_page.cgi
main:
	gcc -g -g -I.  -c -o main.o main.c
	gcc -o main.cgi main.o $(path)events.o $(path)vm.o $(path)process_table.o $(path)process.o $(path)action.o
	chmod og+rx main.cgi
action:
	gcc -g -g -I.  -c -o action_page.o action_page.c
	gcc -o action_page.cgi action_page.o $(path)events.o $(path)vm.o $(path)process_table.o $(path)process.o $(path)action.o
	chmod og+rx action_page.cgi
test:
	gcc -g -g -I.  -c -o create_testtable.o create_testtable.c
	gcc -o create_testtable create_testtable.o $(path)events.o $(path)vm.o $(path)process_table.o $(path)process.o $(path)action.o
	chmod og+rx create_testtable

