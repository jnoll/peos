proc tclf_filecount { pathname } {
       set i 0
       foreach f [exec ls $pathname] {
	  set i [expr $i + 1] 
       }
       expr $i 
}