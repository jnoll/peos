CFLAGS=	-g -g3 -Wall

all:
	gcc ${CFLAGS}  main.c interface.c callbacks.c engine.c \
		glo.c parse.c tree.c html.c viewmenu.c deletemenu.c htmllinkage.c notebook.c -o runpeos \
		`gnome-config --cflags --libs gtk xml2 gal` `pkg-config --cflags --libs gtkhtml-1.1`

gdb: all
		gdb ./runpeos
test:	gcc ${CFLAGS} parseit.c -o test
	./test
		
dist:
	rm -rf peosdir;
	mkdir peosdir;
	cp -r ./*.c ./*h ./image ./gtk Makefile README peos ./peosdir
	tar cvf peos.tar peosdir
	rm -rf peosdir
	
clean:
	rm -rf runpeos peosdir proc* core*
