test: test_run
init:
	cp ../proc_table.dat ../peos_init.tcl ../*.pml .

test_create: init
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "op=create&model=simple"|wc -c`; \
	echo -n "op=create&model=simple" | ../peos.cgi | grep 'pid="2"' > /dev/null && echo "success"

test_run: test_create
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "op=start&pid=2&action=act_2"|wc -c`; \
	echo -n "op=start&pid=2&action=act_2" | ../peos.cgi | grep "act_2"|grep RUN>/dev/null || echo "fail: act_2 not RUNning"
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "op=finish&pid=2&action=act_2"|wc -c`; \
	echo -n "op=finish&pid=2&action=act_2" | ../peos.cgi | grep "act_2"|grep PENDING>/dev/null || echo "fail: act_2 not PENDING"

