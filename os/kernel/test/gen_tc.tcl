#!/usr/bin/env tclsh
proc preamble {tfile pfile {iterations 1} {fill_proc_table 0} {shell ../../shell}} {

    puts $tfile "model: $pfile"
    puts $tfile "shell: ../../shell"
    puts $tfile "iterations: $iterations"
    if {$fill_proc_table} {
	puts $tfile "fill_proc_table: 1"
    }
}

proc action {pfile tfile num_act type} {
    global act 

    set indent [expr {$type == "action" ? "  " : "    "}]

    for {set i 0} {$i < $num_act} {incr i} {
	puts $pfile "${indent}action act_${act} \{\}"
	if {$type != "selection"  || $i == 0} {
	    puts -nonewline $tfile "act_${act} "
	}
	incr act
    }
}

proc nested {inner outer num_act iterations fill_proc_table shell} {
    global act
    set act 0
    set fname ${inner}-in-${outer}-${iterations}-${num_act}
    if {$fill_proc_table} {
	append fname "-fill"
    }
    set pfile [open "${fname}.pml" "w"]
    set tfile [open "${fname}.tc" "w"]
    preamble $tfile "${fname}.pml" $iterations $fill_proc_table $shell

    puts $pfile "process p \{"
    puts $pfile "  $outer \{"

    if {$inner != "action"} {
	puts $pfile "    $inner \{"
    }

    puts -nonewline  $tfile "actions: "
    action $pfile $tfile $num_act $inner

    if {$inner != "action"} {
	puts $pfile "    \}"
    }
    puts $pfile "  \}"
    puts $pfile "\}"

    close $pfile
    close $tfile
}

proc follows {first second num_act iterations fill_proc_table shell} {
    global act
    set act 0
    set fname ${first}-in-${second}-${iterations}-${num_act}
    if {$fill_proc_table} {
	append fname "-fill"
    }
    set pfile [open "${fname}.pml" "w"]
    set tfile [open "${fname}.tc" "w"]
    preamble $tfile "${fname}.pml" $iterations $fill_proc_table $shell

    puts $pfile "process p \{"

    if {$first != "action"} {
	puts $pfile "  $first \{"
    }

    puts -nonewline $tfile "actions: "
    action $pfile $tfile $num_act $first

    if {$first != "action"} {
	puts $pfile "  \}"
    }

    if {$second != "action"} {
	puts $pfile "  $second \{"
    }

    action $pfile $tfile $num_act $second

    if {$second != "action"} {
	puts $pfile "  \}"
    }

    puts $pfile "\}"

    close $pfile
    close $tfile
}


set control [list iteration sequence branch selection]
set fields [list provides requires script agent tool]
set values [list \"a string\" Identifier Identifier.attribute predicate clause]
set ops [list "==" "!=" "<" "<=" ">" "=>"]
set act 0

set fill_proc_table 0

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
    -fill_proc_table {
      set fill_proc_table 1
    }
    -num_act {
	incr i
	set num_act [lindex $argv $i]
    }
  }
  incr i
}

foreach outer $control {
    nested "action" $outer $num_act $iterations $fill_proc_table $shell
    follows "action" $outer $num_act $iterations $fill_proc_table $shell
    follows $outer "action" $num_act $iterations $fill_proc_table $shell

    foreach inner $control {
	nested $outer $inner $num_act $iterations $fill_proc_table $shell
	follows $outer $inner $num_act $iterations $fill_proc_table $shell
    }
}
