process ProjectLog {
	action Log manual {
		agent { "PEOS user" }
		script { "ongoing project log" }
		tool { "create SourceCodeEdit" }
		input { "SourceCodeEdit.pml" }
		requires { "plan/$PROJ/log.body" }
		provides { "plan/$PROJ/out/log.html" }
	}
}
