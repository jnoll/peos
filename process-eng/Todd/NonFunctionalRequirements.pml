process NonFunctionalRequirements {
	action NonFunctionalEdit manual {
		agent { "PEOS user" }
		script { "list of the non-functional requirements" }
		tool { "create SourceCodeEdit" }
		input { "SourceCodeEdit.pml" }
		requires { "requirements/$PROJ/spec.body" }
		provides { "requirements/$PROJ/out/spec.html" }
	}
}
