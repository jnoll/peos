process Architecture {
	sequence {
		action updateWorkArea manual {
			agent { "student" }
			tool { "PEOS Engine" }
			script { "Create and Execute Update Process, filename is your project root." }
			requires { "architecture.body" }
			provides { "architecture.body" }
		}
		branch {
			task CreateArchitecture {
				iteration {
					action checkoutArch manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Checkout Process, filename is architecture.body" }
						requires { "architecture.body" }
						provides { "architecture.body" }
					}
					action editArch manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Edit Process, filename is architecture.body" }
						requires { "architecture.body" }
						provides { "architecture.body" }
					}
					action commitArch manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Commit Process, filename is architecture.body" }
						requires { "architecture.body" }
						provides { "architecture.body" }
					}
				}
			}
			task UpdateRisks {
				iteration {
					action checkoutRisks manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Checkout Process, filename is risks.body" }
						requires { "risks.body" }
						provides { "risks.body" }
					}
					action editRisks manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Edit Process, filename is risks.body" }
						requires { "risks.body" }
						provides { "risks.body" }
					}
					action commitRisks manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Commit Process, filename is risks.body" }
						requires { "risks.body" }
						provides { "risks.body" }
					}
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
}
			
		
