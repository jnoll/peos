proc tclf_spellcheck { filename } {
#	puts stdout "spellcheck arguments: $filename"
	puts stdout [ expr { [exec spell $filename] } ]
#	puts stdout "------------------------------"
	expr { [exec spell $filename] }
}