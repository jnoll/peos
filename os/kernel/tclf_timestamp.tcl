proc timestamp { path1 path2 } {
#        puts stdout "[tclf_timestamp.tcl] Timestamp arguments: $path1 $path2"
        file stat $path1 stat1
        file stat $path2 stat2
        expr { $stat1(mtime) > $stat2(mtime) }
}
