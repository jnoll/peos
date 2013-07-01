pilrc -ro -allowEditID -q ../rsc/AppResources.rcp palmpeos.ro
m68k-palmos-multigen palmpeos.def
m68k-palmos-gcc -c palmpeos-sections.s
m68k-palmos-gcc -Xlinker -M -Xlinker --cref -g AppMain.o  AvailProcesses.o \
	 StartedProcesses.o palmpeos-sections.o -lPalmOSGlue\
	 ../../../os/palm/stdio.o ../../../os/kernel/libpeos.a \
	../../../pml/libpml.a -o palmpeos palmpeos-sections.ld \
	2> palmpeos.map
build-prc palmpeos.def -o palmpeos.prc -n PalmPeos -c plmp palmpeos.ro palmpeos
