
proc timestamp { path1 path2 } {
	puts stdout "timestamp arguments: $path1 $path2"
        #set path1 mymodule.mod
	#set path2 myexec.exe
        #file stat $path1 stat1
        #file stat $path2 stat2
        #return [expr $stat1(mtime) > $stat2(mtime)]
	#$stat1(ino) == $stat2(ino) && \
         #               $stat1(dev) == $stat2(dev)
	 return 55
}
 


#proc timestamp { file1 file2 } {
       # set c [expr sqrt($a * $a + $b * $b)]
  # puts stdout $file1
   #puts stdout $file2
#        file stat $file1 stat1
  #   file stat $file2 stat2
   #  if { $stat1(mtime) > $stat2(mtime)} {
#       return 1
#     } else {
 #      return 0
#   }

#}