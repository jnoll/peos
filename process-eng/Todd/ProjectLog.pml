process ProjectLog {
	action Log manual {
		agent { "PEOS user" }
		script { "ongoing project log" }
		tool { "editor" }
		input { "SourceCodeEdit.pml" }
		requires { "log.body" }
		provides { "log.html" }
	}
}
