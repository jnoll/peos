#######################################################################
## File Information: $Id: uiTest.tcl,v 1.3 1999/05/07 23:01:32 jneuder Exp $
#######################################################################

#####################################################################
#
#   Test Routines For Performing ScreenShots
#
#####################################################################
namespace eval ::uiTest {


}

#####################################################################
## Procedure Name:
## Precondition:
## Postcondition:
## Description:
#####################################################################

proc ::uiTest::test_choice_list {cType} {

    set ::uiValues::choiceType $cType

    if { $cType == "Branch" } {
	set ::uiValues::choiceList [list "branch task 1" "branch task 2" "branch task 3"]
    } elseif { $cType == "Selection" } {
	set ::uiValues::choiceList [list "branch task 1" "branch task 2" "branch task 3" "branch task 4"]
    } else {
	set ::uiValues::choiceList [list "action 1" "action 2" "action 3" "action 4" "action 5"]
    }

    ::dynamicWidgets::makeChoiceList
}


####################################################################
## Procedure Name:
## Precondition:
## Postcondition:
## Description:
#####################################################################


proc ::uiTest::test_process_select {} {
	set ::uiValues::resumeList [list "process 1" "process 2" "process 3" "process 4" "process 5"]
	set ::uiValues::availList [list "process A" "process B" "process C" "process D"]
	::dynamicWidgets::makeProcessSelect
}


####################################################################
## Procedure Name:
## Precondition:
## Postcondition:
## Description:
#####################################################################

proc ::uiTest::test_action {actionType} {

    set ::uiValues::processName example
    set ::uiValues::taskName analyze
    set ::uiValues::taskState started
    set ::uiValues::actionName "develop solution"
    set ::uiValues::actionField [list tarfile 20 "Enter file to extract"]
    set ::uiValues::actionTool "whatchamacallit"
    set ::uiValues::actionRequires "data"
    set ::uiValues::actionProvides "solution"
    set ::uiValues::actionType $actionType

    if { $actionType == "Manual" } {
	set ::uiValues::actionState inactive
	set ::uiValues::actDisplay "Write code to develop a solution for solving problem or call somebody who can"
	set ::uiValues::actionScript "null"
	set ::uiValues::actStep "1 of 6"
    } else {
	set ::uiValues::actionState started
	set ::uiValues::actionDisplay "Use whatchamacallit to get solution for problem"
	set ::uiValues::actionScript "SolveProblem.scr"
	set ::uiValues::actionStep "3 of 6"
    }

    ::dynamicWidgets::makeAction
}

proc ::uiTest::test_iteration_complete { } {
    set ::uiValues::iterComp Iteration3
    ::dynamicWidgets::makeIterationComplete
}

proc ::uiTest::test_info { } {
    set ::uiValues::infoText [list [list "Selected action does not exist. Please notify the PML Modeling Engineer"]]
    ::dynamicWidgets::makeInfo
}
