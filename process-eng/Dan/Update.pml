process Update {
	iteration {
		action update manual {
			agent { "student" }
			tool { "cvs update" }
			script { "Navigate to File Location, update file using cvs tool." }
			requires { "fileName" }
			provides { "updated file" }
		}
		action resolve manual {
			agent { "student" }
			tool { "vi fileName" }
			script { "Resolve file update conflicts using vi." }
			requires { "fileName" }
			provides { "updated file" }
		}
	}
}
