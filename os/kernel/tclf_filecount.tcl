proc tclf_filecount { pathname } {
#	puts stdout "filecount arguments: $pathname"
#	puts stdout [ expr { [exec ls $pathname | wc -l] } ]
        expr { [exec ls $pathname > tclf_filecount_temp] }
	expr { [exec wc -l tclf_filecount_temp] }
}