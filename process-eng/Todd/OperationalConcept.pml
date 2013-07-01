process OperationalConcept {
	action OperationalConceptEdit manual {
		agent { "PEOS user" }
		script { "description of the agent's operational concept" }
		tool { "create SourceCodeEdit" }
		input { "SourceCodeEdit.pml" }
		requires { "requirements/$PROJ/op-concept.body" }
		provides { "requirements/$PROJ/out/op-concept.html" }
	}
}
