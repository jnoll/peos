PalmPEOS.prc: code0000.PalmPEOS.grc tFRM03e8.bin
	build-prc  PalmPEOS.prc "PalmPEOS" IP02 *.grc *.bin

tFRM03e8.bin: PalmPEOS.rcp PalmPEOS.h
	pilrc PalmPEOS.rcp 

code0000.PalmPEOS.grc: PalmEngine.c PalmUI.c PalmVM.c PalmEngine.h PalmUI.h PalmVM.h globals.h
	m68k-palmos-gcc PalmEngine.c PalmUI.c PalmVM.c -o PalmPEOS
	m68k-palmos-obj-res PalmPEOS

clean:
	rm PalmPEOS
	rm *.grc
	rm *.bin
	rm *~


