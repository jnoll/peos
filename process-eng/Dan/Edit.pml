process Edit {
	iteration {
		iteration {
			action modifyFile manual {
				agent { "student" }
				tool { "vi" }
				script { "Make appropriate modifications to file." }
				requires { "filename" }
				provides { "filename" }
			}
			action makeFile manual {
				agent { "student" }
				tool { "make" }
				script { "Use Makefile to create deliverables." }
				requires { "filename" }
				provides { "htmlFile" }
			}
		}
		action testFile manual {
			agent { "student" }
			tool { "lynx" }
			script { "Test the html file using lynx." }
			requires { "htmlFile" }
			provides { "filename" }
		}
	}
}
