CFLAGS=	-g -g3 -Wall
CPATH=	main.c interface.c callbacks.c engine.c \
	glo.c parse.c tree.c html.c viewmenu.c  \
	deletemenu.c htmllinkage.c notebook.c -O -o runpeos \
	`gnome-config --cflags --libs gtk xml2 gal`      \
	`pkg-config --cflags --libs gtkhtml-1.1` 
LDD=	ldd ./runpeos
GREP=	grep '==>'
AWK=	awk '{print $3;}'
XARGS=	xargs libtool gcc ${CPATH} -rpath /usr/local/lib -lm
SORT=	sort -u -o system_library_req

all:
	gcc ${CFLAGS} ${CPATH}

gdb: all
		gdb ./runpeos

manConf:	all
	${LDD} | ${GREP} | ${AWK} | ${XARGS} | ${SORT}
	
	
		
dist:
	rm -rf peosdir;
	mkdir peosdir;
	cp -r ./*.c ./*h ./image ./gtk Makefile README peos ./peosdir
	tar cvf peos.tar peosdir
	rm -rf peosdir
	
clean:
	rm -rf runpeos peosdir proc* core*
