proc tclf_exists { path } {
       # puts stdout "File_exists arguments: $path"
        expr { [file exists $path] }
}
