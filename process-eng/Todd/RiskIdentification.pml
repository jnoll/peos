process RiskIdentification {
	action IdentifyRisks manual {
		agent { "PEOS user" }
		script { "identifies the project risks" }
		tool { "editor" }
		input { "SourceCodeEdit.pml" }
		requires { "risks.body" }
		provides { "risks.html" }
	}
}
