process Architecture {
	sequence {
		action updateWorkArea manual {
			agent { "student" }
			tool { "PEOS Engine" }
			script { "Create and Execute Update Process, the goal is to get a current set of files in your work area." }
			requires { "$HOME/peos" }
			provides { "$HOME/peos is current" }
		}
		branch {
			task CreateArchitecture {
				iteration {
					selection {
						task createDesign {
							action checkoutArch manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script { "Create and Execute Checkout Process on design.body" }
								requires { "architecture/PROJ/design.body" }
								provides { "current architecture/PROJ/design.body" }
							}
							action editArchComponents manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script { "Create and Execute Edit Process, filename is design.body.  Use the arch_components field to describe each component in your design. The appropriate granularity of a component is up to you to decide, but should be at least at the level of major subsystem. List the components using a descrip or itemize list; make each component a separate entry in the list. Describe the functionality of the component, ideally in terms of the requirements it implements, and its behavior (which may be the same thing). The intent is to be able to consult this list in the future to discover what a particular component in the implementation is supposed to do, and also where requirements are allocated. You may also define the interface of each component here, if that makes sense. For example, if you use a traditional C module approach, you could define each module and the functions it exports, in the arch_components field." }
							}
							action editArchInterfaces manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script { "Use the arch_interfaces field to desribe the interfaces between components. This can take several forms, depending on your specific design.  If you define the interface exported by each component along with the component in the arch_components field, then you might just mark this field ``see component definitions'' if the interactions between components are clear from their exported interfaces.  In UML, one implicitly defines interfaces between objects by documenting the messages they send to each other. Similarly, one can document interfaces between traditional modules by document which functions are called by which modules.  If your design includes interfaces that are themselves part of the design (like communication protocols, command-line arguments, language specifications), document them here. For example, the compiled PML language defines the interface between the Compiler and the Virtual Machine; it's a significant enough part of the design that it warrants separate treatment. Likewise, the communication protocol between the Enactment User Interfaces and the PEOS kernel defines the interface between these components, but should have separate treatment." }
							}
							action editArchConfigurations manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script {"The arch_configurations field is intended to document the ways components may be combined into the whole system. If the configuration is obvious from the component interfaces, mark this field ``see interfaces.'' If your design includes some post-implementation configuration of components, describe how functioning systems look in terms of their components." }
							}
							action editArchConstraints manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script {"If the system design has any constraints that must hold, document these in the arch_constraints field. Mark this field ``not applicable'' if appropriate." }
								requires { "current architecture/PROJ/design.body" }
								provides { "modified architecture/PROJ/design.body" }
							}
							action commitArch manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script { "Create and Execute Commit Process, filename is design.body" }
								requires { "modified architecture/PROJ/design.body" }
								provides { "committed architecture/PROJ/design.body" }
							}
						}
						task create_alternatives {
							action checkoutArch manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script { "This task is for the creation of design alternatives.  Please note that this is OPTIONAL. Create and Execute Checkout Process on alternatives.body" }
								requires { "architecture/PROJ/alternatives.body" }
								provides { "current architecture/PROJ/alternatives.body" }
							}
							action editArch manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script { "Create and Execute Edit Process, filename is alternatives.body.  This will use the same format as the design." }
								requires { "current architecture/PROJ/alternatives.body" }
								provides { "modified architecture/PROJ/alternatives.body" }
							}
							action commitArch manual {
								agent { "student" }
								tool { "PEOS Engine" }
								script { "Create and Execute Commit Process, filename is alternatives.body" }
								requires { "modified architecture/PROJ/alternatives.body" }
								provides { "committed architecture/PROJ/alternatives.body" }
							}
						}
					}
				}
			}
			task UpdateRisks {
				iteration {
					action checkoutRisks manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Checkout Process, filename is risks.body" }
						requires { "plan/PROJ/risks.body" }
						provides { "current plan/PROJ/risks.body" }
					}
					action editRisks manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Edit Process, filename is risks.body.  Update your list of top 10 risks in the risks field, ordered by risk exposure." }
						requires { "current plan/PROJ/risks.body" }
						provides { "modified plan/PROJ/risks.body" }
					}
					action commitRisks manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Commit Process, filename is risks.body" }
						requires { "modified plan/PROJ/risks.body" }
						provides { "committed plan/PROJ/risks.body" }
					}
				}
			}
			task UpdateLog {
				iteration {
					action checkoutLog manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Checkout Process, filename is log.body" }
						requires { "plan/PROJ/log.body" }
						provides { "current plan/PROJ/log.body" }
					}
					action editLog manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Edit Process, filename is log.body. Add any notes or decisions to the respective fields. Record the actual effort expended this week in the log_effort field. Log time spent fixing defects that ``break the build'' here." }
						requires { "current plan/PROJ/log.body" }
						provides { "modified plan/PROJ/log.body" }
					}
					action commitLog manual {
						agent { "student" }
						tool { "PEOS Engine" }
						script { "Create and Execute Commit Process, filename is log.body" }
						requires { "modified plan/PROJ/log.body" }
						provides { "committed plan/PROJ/log.body" }
					}
				}
			}
		}
	}
}
