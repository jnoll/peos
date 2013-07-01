#compiler
CC = m68k-palmos-gcc
#combine all application resources
PRC = build-prc
INCLUDE = -I/usr/m68k-palmos
#C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/include/stlport
STDIO_ORIG = -I/usr/m68k-palmos
PEOSINC = -I"../../../os/kernel"  -I"../../../os/palm"   -I"../../.." ${STDIO_ORIG}
LIB = -l ../../../os/kernel/libpeos.a -l ../../../pml/libpml.a

#STUBINC = -I"./StubKernel.h"
STUBLIB = -l ./stubkernel.a

#cd ../../../pml
	#make -f Palmmake
	#cd ../os/kernel
	#make -f Palmmake -lib
	#cd ../../ui/palm/Debug

build:
	$(CC) -DPALM -O0 -g -fexceptions -UNDEBUG  $(INCLUDE) $(PEOSINC) -Wall -c   ../src/AppMain.c ../src/AvailProcesses.c ../src/StartedProcesses.c ../src/CurrentProcess.c
	pilrc -ro -allowEditID -q ../rsc/AppResources.rcp palmpeos.ro
	m68k-palmos-multigen palmpeos.def
	m68k-palmos-gcc -c palmpeos-sections.s
	m68k-palmos-gcc -Xlinker -M -Xlinker --cref -g AppMain.o  AvailProcesses.o \
		StartedProcesses.o CurrentProcess.o palmpeos-sections.o -lPalmOSGlue \
		../../../os/kernel/libpeos.a  ../../../pml/libpml.a \
		../../../os/palm/stdio.o -o palmpeos palmpeos-sections.ld \
		2> palmpeos.map
	build-prc palmpeos.def -o palmpeos.prc -n PalmPeos -c plmp palmpeos.ro palmpeos
#	$(CC) -c -D__PALMOS__=0x05000000 $(INCLUDE) -O0 -g -fexceptions -UNDEBUG -Wall -palmos5r4 -o palmpeos  *.o
#	m68k-palmos-obj-res palmpeos.o
#	$(PILRC) ../rsc/AppResources.rcp 
#	$(PRC)  -c 'STRT' -n PalmPEOS -o palmpeos.prc -t appl *.grc *.bin


stub:
	$(CC) -c -g  ../src/StubKernel.c
	m68k-palmos-ar cru stubkernel.a ./StubKernel.o
	m68k-palmos-ranlib stubkernel.a || true
	
	$(CC) -DPALM -DSTUB -O0 -g -fexceptions -UNDEBUG  $(INCLUDE) $(STUBLIB) -Wall -c   \
	../src/AppMain.c ../src/AvailProcesses.c ../src/StartedProcesses.c ../src/CurrentProcess.c
	pilrc -ro -allowEditID -q ../rsc/AppResources.rcp palmpeos.ro
	m68k-palmos-gcc -Xlinker -M -Xlinker --cref -g AppMain.o  AvailProcesses.o \
		StartedProcesses.o CurrentProcess.o ../../../os/palm/stdio.o ./stubkernel.a  -o palmpeos \
		2> palmpeos.map
	build-prc -o palmpeos.prc -n PalmPeos -c plmp palmpeos.ro palmpeos

clean:  
	rm -rf *.prc *.grc *.o *.bin rm *.s rm *.ro *.ld *.a
	

