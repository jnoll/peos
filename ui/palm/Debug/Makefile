all:
	m68k-palmos-gcc -c -D__PALMOS__=0x05000000 -I"C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/include/stlport" -O0 -g -fexceptions -UNDEBUG -Wall -palmos5r4 -o src/AppMain.o  ../src/AppMain.c
	echo -n src/AppMain.d 'src/' > src/AppMain.d && \
	m68k-palmos-gcc -c -MM -MG -P -w -D__PALMOS__=0x05000000 -I"C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/include/stlport" -O0 -g -fexceptions -UNDEBUG -Wall -palmos5r4  ../src/AppMain.c >> src/AppMain.d

	m68k-palmos-gcc -g -o helloworld2 src/AppMain.o       

	"C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/PalmRC" -target 4.0 -strictTextEncoding -quiet -p PalmOS4 -o rsc/AppResources.trc  ../rsc/AppResources.xrd


	echo -n rsc/AppResources.d rsc/AppResources.trc : ' ' > rsc/AppResources.d && \
	"C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/PalmRC" -quiet -makedeps rsc/AppResources.d.tmp ../rsc/AppResources.xrd && \
	/usr/bin/cygpath -m -f rsc/AppResources.d.tmp | /usr/bin/sed 's/ /\\\ /g' | /usr/bin/tr '\r\n' ' ' >> rsc/AppResources.d && \
	rm -rf rsc/AppResources.d.tmp

	build-prc  -c 'STRT' -n helloworld2 -o buildprc_temp.prc helloworld2 

	"C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/PRCMerge" -quiet -omit pref -o prcmerge_temp.prc buildprc_temp.prc 

	"C:/Program Files/PalmSource/Palm OS Developer Suite/PalmOSTools/PRCMerge" -quiet -dbVersion 1 -dbType 'appl' -dbCreator 'STRT' -n helloworld2 -b     -o helloworld2.prc prcmerge_temp.prc rsc/AppResources.trc  
	rm prcmerge_temp.prc buildprc_temp.prc
	

