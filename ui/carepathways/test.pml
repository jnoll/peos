process test {
	action action1 {
		agent {agent1}
		requires {resource1}
		provides {resource2}
	}
		
	action action2 {
		agent {agent2}
		requires {resource1}
		requires {resource2}
		provides {resource2}
		provides {resource3}
	}	
	
	branch branch1 {
		sequence sequence1 {
			action action3 {
				agent {agent3}
				requires {resource1}
				provides {resource2}
			}
				
			action action4 {
				agent {agent4}
				requires {resource1}
				requires {resource2}
				provides {resource2}
				provides {resource3}
			}
		}
		
		iteration iteration1 {
			action action5 {
				agent {agent5}
				requires {resource1}
				provides {resource5}
			}
				
			action action6 {
				agent {agent6}
				requires {resource1}
				requires {resource5}
				provides {resource4}
				provides {resource5}
			}
		}
	}
	
	selection selection1 {
		action action7 {
			agent {agent1}
			requires {resource1}
			provides {resource2}
		}
			
		action action8 {
			agent {agent2}
			requires {resource1}
			requires {resource2}
			provides {resource2}
			provides {resource3}
		}	
	}
	iteration iteration1 {
		action action9 {
			agent {agent5}
			requires {resource1}
			provides {resource9}
			script {"Script for action9"}
		}
	}
}

		