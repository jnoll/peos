process Milestone2 {
	sequence SourceCodeUpdate {
		action CheckOut executable {
			agent { "PEOS User" }
			tool { "create RepositoryCheckOut" }
			input { "RepositoryCheckOut.pml" }
			requires { "RepositoryCheckOut.pml" }
			provides { "current source code" }
		}

		branch DeliverableCreation {
			action MilestoneOperationalConcept executable {
				agent { "PEOS User" }
				tool { "create OperationalConcept" }
				input { "OperationalConcept.pml" }
				requires { "OperationalConcept.pml" }
				provides { "operational concept" }
			}
			action MilestoneAnalysis executable {
				agent { "PEOS User" }
				tool { "create Analysis" }
				input { "Analysis.pml" }
				requires { "Analysis.pml" }
				provides { "Analysis" }
			}
			action MilestoneRiskIdentification executable {
				agent { "PEOS User" }
				tool { "create RiskIdentification" }
				input { "RiskIdentification.pml" }
				requires { "RiskIdentification.pml" }
				provides { "RiskIdentification" }
			}
			action MilestoneLog executable {
				agent { "PEOS User" }
				tool { "create Log" }
				input { "Log.pml" }
				requires { "Log.pml" }
				provides { "Log" }
			}
		}
		sequence RepositoryUpdate {
			action MilestoneRespositorySynchronize executable {
				agent { "PEOS User" }
				tool { "create RepositorySynchronize" }
				input { "RepositorySynchronize.pml" }
				requires { "RepositorySynchronize.pml" }
				provides { "Synchronized Source Code" }
			}
			action MilestoneRespositoryCheckIn executable {
				agent { "PEOS User" }
				tool { "create RepositoryCheckIn" }
				input { "RepositoryCheckIn.pml" }
				requires { "RepositoryCheckIn.pml" }
				provides { "Updated Repository" } 
			}
		}
	}
}
				
