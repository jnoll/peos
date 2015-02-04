PMLCHECK=../../../pml/check/pmlcheck
INIT_TCL=../../../os/kernel/peos_init.tcl 
test: test_run test_check
init:
<<<<<<< HEAD
	@cp $(INIT_TCL) .
	@cp ../proc_table.dat ../*.pml .
=======
	@rm -f proc_table.dat*
	@cp $(INIT_TCL) .
	@cp  ../*.pml .
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
	@[ -L pmlcheck ] || ln -s $(PMLCHECK) .

test_create: init
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "event=create&model=simple"|wc -c`; \
<<<<<<< HEAD
	echo -n "event=create&model=simple" | ../peos.cgi | grep 'pid="2"' > /dev/null && echo "test_create: success"
=======
	echo -n "event=create&model=simple" | ../peos.cgi | grep 'pid="0"' > /dev/null && echo "test_create: success"
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8

test_run: test_create
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
<<<<<<< HEAD
	export CONTENT_LENGTH=`echo -n "event=start&pid=2&action=act_2"|wc -c`; \
	echo -n "event=start&pid=2&action=act_2" | ../peos.cgi | grep "act_2"|grep RUN>/dev/null || echo "fail: act_2 not RUNning"
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "event=finish&pid=2&action=act_2"|wc -c`; \
	echo -n "event=finish&pid=2&action=act_2" | ../peos.cgi | grep "act_2"|grep PENDING>/dev/null && echo "test_run: success" || echo "fail: act_2 not PENDING"

test_check: init
=======
	export CONTENT_LENGTH=`echo -n "event=start&pid=0&action=act_2"|wc -c`; \
	echo -n "event=start&pid=0&action=act_2" | ../peos.cgi | grep "act_2"|grep RUN>/dev/null || echo "fail: act_2 not RUNning"
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "event=finish&pid=0&action=act_2"|wc -c`; \
	echo -n "event=finish&pid=0&action=act_2" | ../peos.cgi | grep "act_2"|grep PENDING>/dev/null && echo "test_run: success" || echo "fail: act_2 not PENDING"

test_check: #init
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "editedText=process p { "|wc -c`; \
	echo -n "editedText=process p { " | ../check.cgi 2>&1 | grep "stdin:2: syntax" > /dev/null && echo "test_check: success" || echo "fail: pmlcheck didn't report syntax error"

<<<<<<< HEAD
=======
clean: 
	rm -f proc_table.dat
>>>>>>> 71f6a61cb06573d2dbf317e9ee585b978950aef8
