process RepositoryCheckIn {
	action CheckIn executable {
		agent { "PEOS user" }
		script { "commits the source code to the repository" }
		tool { "/usr/bin/cvs" }
		input { "SourceCodeEdit.pml" }
		requires { "modified source code" }
		provides { "updated repository" }
	}
}
