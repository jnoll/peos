process Edit {
	iteration {
		iteration {
			action modifyFile manual {
				agent { "student" }
				tool { "vi file" }
				script { "Make appropriate modifications to file using vi.  After making the appropriate modifications, write and quit vi." }
				requires { "current file" }
				provides { "edited file" }
			}
			action makeFile manual {
				agent { "student" }
				tool { "make" }
				script { "Use Makefile to create deliverables. CD to the directory of the file being edited and type make.  This loop is finished upon a successful make (no errors)." }
				requires { "edited file" }
				provides { "out/file.html" }
			}
		}
		action testFile manual {
			agent { "student" }
			tool { "lynx out/file" }
			script { "Test the html file using lynx. Scroll through your edits and verify that there are no formatting or spelling errors." }
			requires { "out/htmlFile" }
			provides { "modified file" }
		}
	}
}
