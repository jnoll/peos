process RepositoryCheckOut {
	sequence {
		action Login {
			type { executable }
			agent { PEOS User }
			script { logs the agent in the CVS Server }
			tool { Concurrent Versioning System(CVS) }
			requires { username, password, CVS root }
			provides { acces to cvs server }
		}

		action CheckOut {
			type { executable }
			agent { PEOS User }
			script { copies files in the repository to specified location }
			tool { CVS }
			requires { login }
			provides { current version of files in repository }
			input { root directory }
		}
	}
}
