process OperationalConcept {
	action OperationalConceptEdit manual {
		agent { "PEOS user" }
		script { "description of the agent's operational concept" }
		tool { "editor" }
		input { SourceCodeEdit.pml }
		requires { op-concept.body }
		provides { op-concept.html }
	}
}
