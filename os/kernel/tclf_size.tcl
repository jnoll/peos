proc tclf_file_size { path } {
       # puts stdout "File_size arguments: $path"
        file stat $path fstat
        expr { $fstat(size) }
}
