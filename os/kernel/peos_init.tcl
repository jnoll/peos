proc default { path } {
    exists $path
}

proc exists { path } {
    expr [file exists $path]
}

proc filecount { path } {
       set i 0
       foreach f [exec ls $path] {
	  set i [expr $i + 1] 
       }
       expr $i
}

proc filesize { path } {
    expr [file size $path]
}

proc timestamp { path } {
    expr [file mtime $path]
}

proc misspellcount { path } {
    set i 0
    foreach f [exec spell $path] {
        set i [expr $i + 1]
    }
    expr $i
}