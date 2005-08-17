proc default { path } {
    exists $path
}

proc exists { path } {
    expr [file exists $path]
}

proc filecount { path } {
    set i 0
    set e [exists $path]
    if {$e == 1} {
        foreach f [exec ls $path] {
            set i [expr $i + 1]
        }
    }
    expr $i
}

proc filesize { path } {
    set i 0
    set e [exists $path]
    if {$e == 1} {
        set i [file size $path]
   }
    expr $i
}

proc timestamp { path } {
    set i 0
    set e [exists $path]
    if {$e == 1} {
        set i [expr [file mtime $path]]
    }
    expr $i
}

proc misspellcount { path } {
    set i 0
    set e [exists $path]
    if {$e == 1} {
        foreach f [exec spell $path] {
            set i [expr $i + 1]
       }
    }
    expr $i
}