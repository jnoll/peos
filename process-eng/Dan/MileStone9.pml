process MileStone9 {
	sequence {
		action updateWorkArea manual {
			agent { "student" }
			tool { "PEOS Engine" }
			script { "Create and Execute Update Process, filename is your project root." }
			requires { "architecture.body" }
			provides { "architecture.body" }
		}
		task PostMortem {
			action conductPostMortem manual {
				agent { "student" }
				script { "Take the Final." }
				requires { "final" }
				provides { "final" }
			}
		}
		task UpdateLog {
			iteration {
				action checkoutLog manual {
					agent { "student" }
					tool { "PEOS Engine" }
					script { "Create and Execute Checkout Process, filename is log.body" }
					requires { "log.body" }
					provides { "log.body" }
				}
				action editLog manual {
					agent { "student" }
					tool { "PEOS Engine" }
					script { "Create and Execute Edit Process, filename is log.body" }
					requires { "log.body" }
					provides { "log.body" }
				}
				action commitLog manual {
					agent { "student" }
					tool { "PEOS Engine" }
					script { "Create and Execute Commit Process, filename is log.body" }
					requires { "log.body" }
					provides { "log.body" }
				}
			}
		}
	}
}
			
