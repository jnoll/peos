process analysis {
	action StakeholderIdentification {
		type { manual }
		agent { PEOS user }
		script { identifies the stakeholders }
		tool { editor }
		input { SourceCodeEdit.pml}
		requires { analysis.body }
		provides { analysis.html }
	}
}
