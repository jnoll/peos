process analysis {
	action StakeholderIdentification manual {
		agent { "PEOS user" }
		script { "identifies the stakeholders" }
		tool { "create SourceCodeEdit" }
		input { "SourceCodeEdit.pml" }
		requires { "requirements/$PROJ/analysis.body" }
		provides { "requirements/$PROJ/out/analysis.html" }
	}
}
