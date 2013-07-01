process RepositorySynchronize {
	sequence Synchonize {
		action Update executable {
			agent { "PEOS user" }
			script { "updates the existing tree with the repository" }
			tool { "i/usr/bin/cvs" }
			requires { "previously checked out code" }
			provides { "merged source code" }
		}

		action ConflictResolution manual {
			agent { "PEOS user" }
			script { "resolves merging conflicts" }
			tool { "create SourceCodeEdit" }
			input { "SourceCodeEdit.pml" }
			requires { "updated source code from the repository" }
			provides { "source code with conflicts resolved" }
		}
	}
}
