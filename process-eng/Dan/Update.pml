process Update {
	iteration {
		action update manual {
			agent { "student" }
			tool { "CVS" }
			script { "Navigate to File Location, update file using tool." }
			requires { "fileName" }
			provides { "status" }
		}
		action resolve manual {
			agent { "student" }
			tool { "vi" }
			script { "Resolve file conflicts using vi." }
			requires { "fileName" }
			provides { "fileName" }
		}
	}
}
