process develop {
	iteration {
		action edit  {
			requires { code.type == "module" }
			provides { code.status == "modified" }
		}
		action compile {
			requires { code.status == "modified" }
			provides { progA.type == "executable" }
		}
		action test  {
			requires { progA }
			provides { progA.status == "tested" }
		}	
		selection {
			action debug  {
				requires { progA.status == "tested" }
				provides { progA.status == "debugged" }
			}
			action check_in  {
				requires { progA.status == "tested" }
				provides { progA.status == "checked_in" }
			}
		}
	}
}
