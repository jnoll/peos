#compiler
CC = m68k-palmos-gcc
#resource compiler
PILRC = pilrc
#combine all application resources
PRC = build-prc
INCLUDE = -I"C:\Program Files\PalmSource\Palm OS Developer Suite\sdk-5r4\include"
#C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/include/stlport
PEOSINC = -I ../../../os/kernel  -I ../../../os/palm   -I ../../../pml
LIB = -l libpml -l libpeos


#cd ../../../pml
	#make -f Palmmake
	#cd ../os/kernel
	#make -f Palmmake -lib
	#cd ../../ui/palm/Debug

all:
	
	
	
	$(CC) -O0 -g -fexceptions -UNDEBUG  $(INCLUDE)  -Wall -palmos5r4 -o palmpeos.o  ../src/AppMain.c ../src/AvailProcesses.c ../src/StartedProcesses.c

#	$(CC) -c -D__PALMOS__=0x05000000 $(INCLUDE) -O0 -g -fexceptions -UNDEBUG -Wall -palmos5r4 -o palmpeos  *.o

	m68k-palmos-obj-res palmpeos.o
	
	$(PILRC) ../rsc/AppResources.rcp 

	$(PRC)  -c 'STRT' -n PalmPEOS -o palmpeos.prc -t appl *.grc *.bin


clean:  
	rm -rf *.prc *.grc *.o *.bin
	

