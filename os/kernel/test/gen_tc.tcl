#!/usr/bin/env tclsh

proc open_pfile {fname} {
    global indent act_id
    set output [open ${fname}.pml "w"]
    puts $output "process $fname \{"
    set act_id 0
    set indent 0
    return $output
}

proc close_pfile {stream} {
    global indent

    puts $stream "\}"
    incr indent -1
    close $stream
}

proc indent {stream} {
    global indent
    for {set i 0} {$i < $indent} {incr i} {
	puts -nonewline $stream " "
    }
}

proc preamble {tfile pfile {shell ../kernel_wrapper}} {
    
    puts $tfile "model: ${pfile}.pml"
    puts $tfile "shell: $shell"
    puts $tfile "script:"
}

proc action {pfile num_act} {
    global act_id

    set actions {}
    for {set i 0} {$i < $num_act} {incr i} {
	set act "act_${act_id}"
	puts $pfile "[indent $pfile]action $act \{\}"
	lappend actions $act
	incr act_id
    }
    return $actions
}

proc control_struct {name pfile num_act} {
    global indent
    puts $pfile "[indent $pfile]${name}  \{"
    incr indent
    set actions [action $pfile $num_act]
    incr indent -1
    puts $pfile "[indent $pfile]\}"
    return $actions
}

proc nest {inner outer pfile num_act} {
    global indent iteration_workaround

    puts $pfile "[indent $pfile]${outer}  \{"
    incr indent
    if {$inner == "iteration" \
	&& [info exists iteration_workaround] && $iteration_workaround > 0} {
	    puts $pfile "[indent $pfile]sequence \{"
	    incr indent
    }

    set actions [$inner $pfile $num_act]
    if {$inner == "iteration" \
	&& [info exists iteration_workaround] && $iteration_workaround > 0} {
	    puts $pfile "[indent $pfile]\}"
	    incr indent -1
    }

    incr indent -1
    puts $pfile "[indent $pfile]\}"

    if {$outer == "iteration"\
	&& [info exists iteration_workaround] && $iteration_workaround > 0} {
	lappend actions [action $pfile 1]
    }

    return $actions
}

proc follow {first second pfile num_act} {
    global indent

    set actions [concat [$first $pfile $num_act] [$second $pfile $num_act]]
    return $actions
}

proc sequence {pfile num_act} {
    return [control_struct "sequence" $pfile $num_act]
}

proc iteration {pfile num_act} {
    global iteration_workaround 

    set actions  [control_struct "iteration" $pfile $num_act]
    if {[info exists iteration_workaround] && $iteration_workaround > 0} {
	lappend actions [action $pfile 1]
    }
    return $actions
}

proc branch {pfile num_act} {
    return [control_struct "branch" $pfile $num_act]
}

proc selection {pfile num_act} {
    set actions [control_struct "selection" $pfile $num_act]
    # Select the middle action.
    return [lindex $actions [expr {[llength $actions]/2}]]
}

proc emit_act {tfile pid act} {
    puts $tfile "assert $pid $act READY"
    puts $tfile "notify $pid $act start"
    puts $tfile "assert $pid $act RUN"
    puts $tfile "notify $pid $act finish"
}

proc emit_concurrent {tfile pname actions iterations} {
    global proc_table_size
    set i 0
    while {$i < $iterations} {
	for {set j 0} {$j < $proc_table_size && ($i + $j) < $iterations} {incr j} {
	    puts $tfile "create_proc $pname"
	}
	for {set j 0} {$j < $proc_table_size && ($i + $j) < $iterations} {incr j} {
	    foreach act $actions {
		emit_act $tfile $j $act
	    }
	    puts $tfile "assert_done $pid $pname"
	}
	incr i $j
    }
}

proc emit_sequential {tfile pname actions iterations} {
    set pid 0
    for {set i 0} {$i < $iterations} {incr i} {
	puts $tfile "create_proc $pname"
	foreach act $actions {
	    emit_act $tfile $pid $act
	}
	puts $tfile "assert_done $pid $pname"
    }
}

proc emit_tc {pname actions mode} {
    global iterations shell

    set tfile [open ${pname}_${iterations}.tc "w"]

    puts $tfile "model: ${pname}.pml"
    puts $tfile "shell: $shell"
    puts $tfile "script:"

    switch -glob $mode {
	par* -
	conc* {
	    emit_concurrent $tfile $pname $actions $iterations
	}

	default -
	seq* {
	    emit_sequential $tfile $pname $actions $iterations
	}
	close $tfile
    }
}


proc baseline {{mode sequential}} {
    global indent num_act

    set pname "baseline_${num_act}"
    set pfile [open_pfile $pname]
    incr indent
    set actions [action $pfile $num_act]
    close_pfile $pfile

    emit_tc $pname $actions $mode
}


proc inventory {{mode sequential}} {
    global indent num_act control

    foreach construct $control {
	set pname "${construct}_${num_act}"
	set pfile [open_pfile $pname]
	incr indent
	set actions [$construct $pfile $num_act]
	close_pfile $pfile

	emit_tc $pname $actions $mode
    }
}

proc nested {{mode sequential}} {
    global indent num_act control

    foreach outer $control {
	foreach inner $control {
	    set pname "${inner}_in_${outer}_${num_act}"
	    set pfile [open_pfile $pname]
	    incr indent
	    set actions [nest $inner $outer $pfile $num_act]
	    close_pfile $pfile

	    emit_tc $pname $actions $mode
	}
    }
}

proc follows {{mode sequential}} {
    global indent num_act control

    foreach first $control {
	foreach second $control {
	    set pname "${first}_${second}_${num_act}"
	    set pfile [open_pfile $pname]
	    incr indent
	    set actions [follow $first $second $pfile $num_act]
	    close_pfile $pfile
	    emit_tc $pname $actions $mode
	}
    }
}

proc combinations {{mode sequential}} {
    nested $mode
    follows $mode
}

set control [list iteration sequence branch selection]
set fields [list provides requires script agent tool]
set values [list \"a string\" Identifier Identifier.attribute predicate clause]
set ops [list "==" "!=" "<" "<=" ">" "=>"]
set act_id 0

set iterations 1
set proc_table_size 10
set num_act 1
set tests {}
set shell "../kernel_wrapper"
set mode sequential
set iteration_workaround 1
set i 0
while {$i < $argc} {
    set arg [lindex $argv $i]
    switch -exact -- $arg {
	-shell {
	    incr i
	    set shell [lindex $argv $i]
	}
	-iterations {
	    incr i
	    set iterations [lindex $argv $i]
	}
	-num_act {
	    incr i
	    set num_act [lindex $argv $i]
	}
	-baseline {
	    lappend tests baseline
	}
	-inventory {
	    lappend tests inventory
	}
	-combinations {
	    lappend tests combinations
	}
	-concurrent {
	    set mode concurrent
	}
    }
    incr i
}

foreach test $tests {
    $test $mode
}
