process RiskIdentification {
	action IdentifyRisks manual {
		agent { "PEOS user" }
		script { "identifies the project risks" }
		tool { "create SourceCodeEdit" }
		input { "SourceCodeEdit.pml" }
		requires { "plan/$PROJ/risks.body" }
		provides { "plan/$PROJ/out/risks.html" }
	}
}
