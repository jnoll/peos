proc filecount { pathname } {
#	puts stdout "filecount arguments: $pathname"
#	puts stdout [ expr { [exec ls $pathname | wc -l] } ]
	expr { [exec ls $pathname | wc -l] }
}