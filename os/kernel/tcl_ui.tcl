#!/usr/bin/tclsh
# PEOS UI using Tcl/Tk
# Pavel Pozdnyakov pavelpoz@yahoo.com 
# 11/22/2004
#

proc Parse_XML {} {
    set xmlfile proc_table.dat.xml
    set result ""
    if [catch {open $xmlfile r} fileId] {
	puts stderr "Cannot open: $fileId"

    } else {
	# Read and process the file, then...
        puts stdout "Reading $xmlfile"
        while {[gets $fileId result] >= 0} {
	# Process line
        set pid -1
	set model ""
	set status ""
	set match ""
        set name ""
	set state ""
	set value ""
	set qualifier ""
	set fp "\t\t"
	    switch -regexp -- $result \
		^<process_table.* { 
		    puts stdout "PROCESS_TABLE: $result" 
		} \
		^<process.* {
		    regexp {.*?\spid="(\d+)".*model="(.*)".*status="(\d+)"} $result match pid model status
		    puts stdout "==============================================="
		    puts stdout "\[PROCESS $pid\]\n|\t$model\n|\tstatus=$status"
		    set fp "\t\t"
		} \
	        ^</process.* {    
		    set fp ""
		} \
		^<iteration.* { 
		    #puts stdout "${fp}ITERATION: $result" 
		} \
		^<action.* { 
		    set fp "------"
		    regexp {.*?\sname="(.*)".*state="(.*)"} $result match name state
		    puts stdout "+${fp}\[$name $state\]"
		} \
		^<req_resource.* {
                    set fp "|\t\t|____" 
		    regexp {.*?\sname="(.*)".*value="(.*)".*qualifier="(.*)"} $result match name value qualifier
		    puts stdout "${fp}\[-\] $name=$value  $qualifier"
		} \
		^<prov_resource.* {
		    set fp "|\t\t|____"
		    regexp {.*?\sname="(.*)".*value="(.*)".*qualifier="(.*)"} $result match name value qualifier
		    puts stdout "${fp}\[+\] $name=$value  $qualifier"
		} \
		^<script.* { 
		    set fp "|\t\t|     "
		    set one 0
                    puts -nonewline stdout "|\t\t|________script: "
		    flush stdout
                    while {[gets $fileId result] >= 0 && [regexp {</script>} $result match] == 0 } {
			incr one
			if {$one > 1} {
			    puts -nonewline stdout "$fp"
			    flush stdout
		        }
			regsub -all {&quot;+} $result \" result
			regsub -all {&lt;+} $result \< result
			regsub -all {&gt;+} $result \> result
			puts stdout "$result"
                    }
		}
	}
        puts stdout "Closing $xmlfile"
	close $fileId
    }
    
}

proc Parse_DAT {} {
}
proc Command_Process {inCmd outCmd} {
    set i [eval $inCmd]
    eval $outCmd {$i $i}
}
proc Command_Read {{in stdin}} {
    if {[eof $in]} {
	if {$in != "stdin"} {
	    close $in
	}
	return {}
    }
    return [gets $in]
}

proc Command_Display {file result code} {
    set peoskernel "./peos"
    set caught ""
    set match 0

    set mat ""
    set pid ""
    set resname ""
    set value ""
   
    set model ""
    set actionname ""
    set event0 ""
    switch -regexp -- $result \
	-c.+ { 
		puts stdout "started with -c: $result";
		puts stdout "[regexp {[-]c\s+(\S+)} $result mat model]"
		catch { exec $peoskernel -c $model  } caught
		puts stdout "new process start caught:\n$caught"
	} \
	-n.+ { 
		puts stdout "started with -n: $result";
		puts stdout "[regexp {[-]n\s+(\S+)\s+(\S+)\s+(\S+)} $result mat pid actionname event0]"
		catch { exec $peoskernel -n $pid $actionname $event0  } caught
		puts stdout "start action caught:\n$caught"
	} \
	-u.* { 
	    puts stdout "started with -u: $result"
	    catch { exec $peoskernel -u } caught
            puts stdout "update all states of all processes caught:\n$caught"
	} \
	-i.* { 
		puts stdout "started with -i: $result"
	    	catch { exec $peoskernel -i} caught
	    	puts stdout "instances caught:\n$caught"
	} \
	-r.+ { 
	    puts stdout "started with -r: $result"
	    puts stdout "[regexp {[-]r\s+(\S+)\s+(\S+)\s+(\S+)} $result mat pid resname value]"
            puts stdout "$pid,$resname,$value\n"
	    catch { exec $peoskernel -r $pid $resname $value  } caught
            puts stdout "binding command caught:\n$caught"
	} \
	-d.+ { 
		puts stdout "started with -d: $result"; 
		puts stdout "[regexp {[-]d\s+(\d+)} $result mat pid]"
		catch { exec $peoskernel -d $pid } caught
		puts stdout "help caught:\n$caught"
	} \
        -h.* { 
		puts stdout "started with -h: $result";
		catch { exec $peoskernel -h} caught
		puts stdout "help caught:\n$caught"
	}
   
} 


Parse_XML
puts -nonewline "Enter a kernel command, i.e. -h for help\n"
puts -nonewline stdout "peos_ui>"
flush stdout 
while {![eof stdin]} {
	Command_Process {Command_Read stdin} \
	{Command_Display stdout}
	Parse_XML
	flush stdout
	puts -nonewline "Enter a kernel command, i.e. -h for help\n"
	puts -nonewline stdout "peos_ui>"
	flush stdout 
}

