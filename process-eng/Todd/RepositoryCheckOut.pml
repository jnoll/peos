process RepositoryCheckOut {
	sequence {
		action Login executable {
			agent { "PEOS User" }
			script { "logs the agent in the CVS Server" }
			tool { "/usr/bin/cvs" }
			requires { "username, password, CVS root" }
			provides { "access to CVS server" }
		}

		action CheckOut executable {
			agent { "PEOS User" }
			script { "copies files in the repository to specified location" }
			tool { "/usr/bin/cvs" }
			requires { "login" }
			provides { "current version of files in repository" }
			input { "root directory" }
		}
	}
}
