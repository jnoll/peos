process RepositorySynchronize {
	sequence Synchonize {
		action Update executable {
			agent { "PEOS user" }
			script { "updates the existing tree with the repository" }
			tool { "CVS" }
			requires { "previously checked out code" }
			provides { "merged source code" }
		}

		action ConflictResolution manual {
			agent { "PEOS user" }
			script { "resolves merging conflicts" }
			tool { "editor" }
			input { "SourceCodeEdit.pml" }
			requires { "updated source code from the repository" }
			provides { "source code with conflicts resolved" }
		}
	}
}
