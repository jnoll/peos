process Commit {
	sequence {
		action checkIn manual {
			agent { "student" }
			tool { "cvs commit" }
			script { "Commit file using CVS. An editor window will appear, enter a one line description of the changes that were made using simple vi, then write and quit." }
			requires { "modified file" }
			provides { "committed file" }
		}
	}
}
