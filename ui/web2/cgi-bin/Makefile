path=../../../os/kernel/
libs=../../../pml/libpml.a

all:
	gcc -g -g -I../..  -c -o main.o main.c
	gcc -o main.cgi main.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx main.cgi
	gcc -g -g -I../..  -c -o action_page.o action_page.c
	gcc -o action_page.cgi action_page.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx action_page.cgi
	gcc -g -g -I../..  -c -o create.o create.c
	gcc -o create.cgi create.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx create.cgi
	gcc -g -g -I../..  -c -o do.o do.c
	gcc -o do.cgi do.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx do.cgi
	gcc -g -g -I../..  -c -o do_create.o do_create.c
	gcc -o do_create.cgi do_create.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx do_create.cgi
main:
	gcc -g -g -I../..  -c -o main.o main.c
	gcc -o main.cgi main.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx main.cgi
action:
	gcc -g -g -I../..  -c -o action_page.o action_page.c
	gcc -o action_page.cgi action_page.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx action_page.cgi
create:
	gcc -g -g -I../..  -c -o create.o create.c
	gcc -o create.cgi create.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx create.cgi
do:
	gcc -g -g -I../..  -c -o do.o do.c
	gcc -o do.cgi do.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx do.cgi
test:
	gcc -g -g -I.  -c -o create_testtable.o create_testtable.c
	gcc -o create_testtable create_testtable.o $(path)events.o $(path)graph_engine.o $(path)process_table.o $(path)process.o $(path)action.o $(libs)
	chmod og+rx create_testtable
	test_main.sh
	test_action_page.sh
	test_create.sh
	test_do.sh
	test_docreate.sh

