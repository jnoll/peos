process Checkout {
	sequence {
		action getFile manual {
			agent { "student" }
			tool { "CVS" }
			script { "Use cvs to checkout file." }
			requires { "filename" }
			provides { "filename" }
		}
	}
}
