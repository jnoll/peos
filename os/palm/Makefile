PalmPEOS.prc: code0000.PalmPEOS.grc tFRM03e8.bin
	build-prc  PalmPEOS.prc "PalmPEOS" IP02 *.grc *.bin

tFRM03e8.bin: PalmPEOS.rcp PalmPEOS.h
	pilrc PalmPEOS.rcp 

code0000.PalmPEOS.grc: PalmVM.c PalmEngine.c PalmUI.c  PalmEngine.h PalmUI.h PalmVM.h globals.h
	m68k-palmos-gcc -g PalmVM.c PalmEngine.c PalmUI.c  -o PalmPEOS
	m68k-palmos-obj-res PalmPEOS

clean:
	rm PalmPEOS
	rm *.grc
	rm *.bin
	rm *~


