PMLCHECK=../../../pml/check/pmlcheck
INIT_TCL=../../../os/kernel/peos_init.tcl 
test: test_run test_check
init:
	@rm -f proc_table.dat*
	@cp $(INIT_TCL) .
	@cp  ../*.pml .
	@[ -L pmlcheck ] || ln -s $(PMLCHECK) .

test_create: init
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "event=create&model=simple"|wc -c`; \
	echo -n "event=create&model=simple" | ../peos.cgi | grep 'pid="0"' > /dev/null && echo "test_create: success"

test_run: test_create
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "event=start&pid=0&action=act_2"|wc -c`; \
	echo -n "event=start&pid=0&action=act_2" | ../peos.cgi | grep "act_2"|grep RUN>/dev/null || echo "fail: act_2 not RUNning"
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "event=finish&pid=0&action=act_2"|wc -c`; \
	echo -n "event=finish&pid=0&action=act_2" | ../peos.cgi | grep "act_2"|grep PENDING>/dev/null && echo "test_run: success" || echo "fail: act_2 not PENDING"

test_check: #init
	@export REQUEST_METHOD=POST; \
	export CONTENT_TYPE="application/x-www-form-urlencoded"; \
	export CONTENT_LENGTH=`echo -n "editedText=process p { "|wc -c`; \
	echo -n "editedText=process p { " | ../check.cgi 2>&1 | grep "stdin:2: syntax" > /dev/null && echo "test_check: success" || echo "fail: pmlcheck didn't report syntax error"

clean: 
	rm -f proc_table.dat