process Milestone2 {
	sequence SourceCodeUpdate {
		action CheckOut executable {
			agent { "PEOS User" }
			tool { "create RepositoryCheckOut" }
			input { "RepositoryCheckOut.pml" }
			requires { "RepositoryCheckOut.pml" }
			provides { "current source code" }
		}

		branch RequirementsSpecification {
			action MilestoneFunctionalRequirements executable {
				agent { "PEOS User" }
				tool { "create FunctionalRequirements" }
				input { "FunctionalRequirements.pml" }
				requires { "FunctionalRequirements.pml" }
				provides { "functional requirements" }
			}
			action MilestoneNonFunctionalRequirements executable {
				agent { "PEOS User" }
				tool { "create NonFunctionalRequirements" }
				input { "NonFunctionalRequirements.pml" }
				requires { "NonFunctionalRequirements.pml" }
				provides { "NonFunctionalRequirements" }
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
				
