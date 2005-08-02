proc default { path } {
    exists $path
}

proc exists { path } {
    expr { [file exists $path] }
}

proc filecount { path } {
       set i 0
       foreach f [exec ls $path] {
	  set i [expr $i + 1] 
       }
       expr $i 
}

proc filesize { path } {
    file stat $path fstat
    expr { $fstat(size) }
}

#proc tclf_spellcheck { filename } {
#    expr { [exec spell $filename] }
#}

proc timestamp { path } {
    file stat $path stat
    expr { $stat(mtime) }
}