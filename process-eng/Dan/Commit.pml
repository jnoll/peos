process Commit {
	sequence {
		action checkIn manual {
			agent { "student" }
			tool { "CVS" }
			script { "Commit file using CVS." }
			requires { "fileName" }
		}
	}
}
