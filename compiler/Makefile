# This make file compiles the "compiler" modules to generate "output"  
# executable. 
 
# c compiler. 
CC = cc 
 
# C flags.  
CFLAGS = -g 
 
GDBMLIB = -lgdbm 
 
# List of compiler objects. 
OBJS = datadict.o io_cpml.o cmplr_mn.o par_fun.o pml_msg.o
 
# List of compiler source.  
SOURCES = datadict.c io-cpml.c cmplr_mn.c par_fun.c 

# Location of executable
INSTALLDIR = ../bin

# 
# Compile source 
# 
 
#simple.all:compiler 
#	@ echo $1 done 

 
.c: 
	$(CC) $(CFLAGS) $(UNIXFLAG) -o $@ $< $(GDBMLIB) 
 
.c.o: 
	$(CC) -c $(CFLAGS) $(UNIXFLAG) $< 
#	lint -c -o compiler -Aa $< 1 > bugs 
 
# Compile code  
all: pmlc compiler install

compiler: $(OBJS) 
	rm -f $@ 
	$(CC) -Aa -g -v -o $@ $(OBJS) $(LIBS) $(GDBMLIB) 
#	lint  *.ln   > lint_list  
 
pmlc: $(OBJS) 
	rm -f $@ 
	$(CC) -Aa -g -v -o $@ $(OBJS) $(LIBS) $(GDBMLIB) 
#	lint  *.ln   > lint_list  
 
install: pmlc compiler
	mv -f pmlc ${INSTALLDIR}
	mv -f compiler ${INSTALLDIR}

clean: 
	rm -f *.o core list bugs lint_list *.ln 
 
