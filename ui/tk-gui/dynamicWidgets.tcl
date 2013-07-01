#######################################################################
## File Information: $Id: dynamicWidgets.tcl,v 1.5 1999/05/07 19:05:45 jneuder Exp $
#######################################################################

package provide dynamicWidgets 1.0

# dynamicWidgets script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sotware Engin.) Spring 1999, Dr. Noll

namespace eval ::dynamicWidgets {

}

####################################################################
## Procedure Name: populateListbox
## Precondition:	a listbox and a list of items must exist
## Postcondition:	the listbox contains the items in the list
## Description: the procedure reads the list and inserts each item
## Arguments: a listbox and the list of items
##
#####################################################################

proc ::dynamicWidgets::populateListbox {lbox plist} {
	set len [llength $plist]

	for {set i 0} {$i < $len} {incr i} {
		set temp [lindex $plist $i]
		eval $lbox insert end {$temp}
	}
}


####################################################################
## Procedure Name: makeChoiceList
## Precondition: list of choices must exist
## Postcondition: a screen with a listbox of choices has been created
## Description: procedure creates listbox of choices and adds button to
##		    trigger response message
## Arguments: none
##
#####################################################################

proc ::dynamicWidgets::makeChoiceList {} {

	# redraw right frame to destroy any widgets currently on screen
	::persistentWidgets::redrawRightFrame
#	::persistentWidgets::redrawLeftFrame

	listbox .right.lbox -borderwidth 1 -relief sunken -selectmode single \
		-background white -yscrollcommand ".right.scrlLbox set"

	populateListbox .right.lbox $::uiValues::choiceList

	# create proper labels
	if {$::uiValues::choiceType == "Branch"} {
		set lblText "Branches"
		set dirText "Select a Branch"
	} elseif {$::uiValues::choiceType == "Action"} {
		set lblText "Actions"
		set dirText "Select an Action"
	} else {
		set lblText "Selections"
		set dirText "Select a Selection"
	}

	label .right.lbLabel -text $lblText -relief raised
	label .right.directions -text $dirText -font biglabelfont

	scrollbar .right.scrlLbox -command ".right.lbox yview"

	# make button to send selected choice back to engine
	button .right.open -text Open \
	    -command {::inputValidator::getChoiceSelected .right.lbox}

	place .right.directions -relx 0.33 -rely 0.03
	place .right.lbLabel -relx 0.2 -rely 0.11 -relheight 0.04 -relwidth 0.6
	place .right.scrlLbox -relx 0.76 -rely 0.15 -relheight 0.6 -relwidth 0.04
	place .right.lbox -relx 0.2 -rely 0.15 -relheight 0.6 -relwidth 0.6 -anchor nw
	place .right.open -relx 0.45 -rely 0.85 -width 70 -height 30 -anchor nw

	::persistentWidgets::configStatusbar "Ready"
}


####################################################################
## Procedure Name: makeProcessSelect
## Precondition: two lists of processes must exist
## Postcondition: a screen showing resumable and avilable processes
##			has been created
## Description: procedure creates two listboxes of processes and a
##		    command button to trigger the message response
## Arguments: none
##
#####################################################################

proc ::dynamicWidgets::makeProcessSelect {} {

	# redraw right frame to destroy any widgets currently on screen
	::persistentWidgets::redrawRightFrame
#	::persistentWidgets::redrawLeftFrame

	listbox .right.lbox1 -borderwidth 1 -relief sunken  \
	    -selectmode single -background white \
	    -yscrollcommand ".right.scrlLbox1 set" \
	    -xscrollcommand ".right.scrlLXbox1 set"

	listbox .right.lbox2 -borderwidth 1 -relief sunken \
	    -selectmode single -background white \
	    -yscrollcommand ".right.scrlLbox2 set" \
	    -xscrollcommand ".right.scrlLXbox2 set"

	populateListbox .right.lbox1 $::uiValues::resumeList
	populateListbox .right.lbox2 $::uiValues::availList

	label .right.directions -text "Select a Process" -font biglabelfont
	label .right.lb1Label -text "Resumable Processes" -relief raised
	label .right.lb2Label -text "Available Processes" -relief raised

	scrollbar .right.scrlLbox1 -command ".right.lbox1 yview"
	scrollbar .right.scrlLbox2 -command ".right.lbox2 yview"
	scrollbar .right.scrlLXbox1 -orient horizontal -command ".right.lbox1 xview"
	scrollbar .right.scrlLXbox2 -orient horizontal -command ".right.lbox2 xview"

	button .right.open -text Open -command \
	    {::inputValidator::getProcessSelected .right.lbox1 .right.lbox2}

	place .right.directions -relx 0.33 -rely 0.03
	place .right.lb1Label -relx 0.1 -rely 0.11 -relheight 0.04 -relwidth 0.35
	place .right.lb2Label -relx 0.55 -rely 0.11 -relheight 0.04 -relwidth 0.35

	place .right.scrlLbox1 -relx 0.41 -rely 0.15 -relheight 0.6 -relwidth 0.04
	place .right.scrlLbox2 -relx 0.86 -rely 0.15 -relheight 0.6 -relwidth 0.04

	place .right.scrlLXbox1 -relx 0.1 -rely 0.75 -relheight 0.04 -relwidth 0.35
	place .right.scrlLXbox2 -relx 0.55 -rely 0.75 -relheight 0.04 -relwidth 0.35

	place .right.lbox1 -relx 0.1 -rely 0.15 -relheight 0.6 -relwidth 0.35 -anchor nw
	place .right.lbox2 -relx 0.55 -rely 0.15 -relheight 0.6 -relwidth 0.35 -anchor nw

	place .right.open -relx 0.45 -rely 0.85 -width 70 -height 30 -anchor nw

	::persistentWidgets::configStatusbar "Ready"
}


####################################################################
## Procedure Name: makeRadioButtons
## Precondition: action procedure has been called
## Postcondition: radio buttons showing state status have been created
## Description: creates radio button and selects current action state
##		    as default value
## Arguments: none
##
## Needed feature: check to make sure value has changed before
## sending a message to engine
#####################################################################

proc ::dynamicWidgets::makeRadioButtons {} {
	# destroy action state label widget currently in place
	destroy .left.actionstate
        destroy .left.inactive
        destroy .left.started
        destroy .left.finished
        destroy .left.update

	# radiobuttons are placed in .left frame
	radiobutton .left.inactive -text "Inactive" -variable newActionState \
		-value "inactive"
	radiobutton .left.started -text "Started" -variable newActionState \
		-value "started"
	radiobutton .left.finished -text "Finished" -variable newActionState \
		-value "finished"

	button .left.update -text "Update" -command \
	    {::inputValidator::getRadioButtonResponse $newActionState}

	# set the selected radiobutton based on the current action state
	if {$::uiValues::actionState == "inactive" } {
		.left.inactive select
	} elseif {$::uiValues::actionState == "started" } {
		.left.started select
	} else {
		.left.finished select
	}

	place .left.inactive -relx .06 -rely 0.63 -anchor nw
	place .left.started -relx .06 -rely 0.68 -anchor nw
	place .left.finished -relx .06 -rely 0.73 -anchor nw
	place .left.update -relx 0.05 -rely 0.79 -relwidth 0.9 -relheight 0.07

}


####################################################################
## Procedure Name: makeAction
## Precondition:
## Postcondition: an action screen has been created
## Description: procedure updates system state varibales, creates a
##		    text widgets describing what action does, sets
##		    action state radiobuttons, handles input for needed
##		    fields, and lists available scripts for the user
## Arguments: none
##
## Limitations: can only handle one 'field' for entry
#####################################################################


proc ::dynamicWidgets::makeAction {} {

	variable defaultFieldWidth 20

	# redraw frames to destroy any widgets currently on screen
	::persistentWidgets::redrawRightFrame
#	::persistentWidgets::redrawLeftFrame

	# draw radiobuttons with state information
	makeRadioButtons

	# set all System State labels
	::persistentWidgets::configProcess $::uiValues::processName
	::persistentWidgets::configTaskName $::uiValues::taskName
	::persistentWidgets::configTaskState $::uiValues::taskState
	::persistentWidgets::configActionName $::uiValues::actionName
	::persistentWidgets::configActionStep $::uiValues::actionStep

	label .right.lblType -text "$::uiValues::actionType Action" -font headerfont

	text .right.tDisplay -wrap word -yscrollcommand ".right.scrlDisplay set"
	.right.tDisplay insert 1.0 "Action Description:\n$::uiValues::actionDisplay\n\nTool:\n$::uiValues::actionTool\n\nRequires:\n$::uiValues::actionRequires\n\nProvides:\n$::uiValues::actionProvides"

	scrollbar .right.scrlDisplay -command ".right.tDisplay yview"

	frame .right.lower -borderwidth 2 -relief groove

#	 set ::uiValues::fieldType [lindex $::uiValues::actionField 0]
#	 set tempWidth [lindex $::uiValues::actionField 1]
#	 if {[string compare $tempWidth ""]} then {
#	    set fieldWidth $tempWidth
#	 } else {
#	    set fieldWidth $defaultFieldWidth
#	 }
#	 set fieldText [lindex $::uiValues::actionField 2]

#	 if {$::uiValues::fieldType != "null"} {
#		 label .right.lower.lblField -text "$fieldText:"
#		 entry .right.lower.entField -width $fieldWidth
#		 button .right.lower.bField -text Open -command {::inputValidator::getManualEntry}
#
#		 place .right.lower.lblField -relx 0.05 -rely 0.05
#		 place .right.lower.entField -relx 0.05 -rely 0.27
#		 place .right.lower.bField -relx 0.05 -rely 0.55 -width 70 -height 30
#	 }

	if {$::uiValues::actionType == "Executable"} {
		label .right.lower.lblScript -text "Available Script:"
		label .right.lower.script -text $::uiValues::actionScript
		button .right.lower.bScript -text Execute -command {::inputValidator::getExecScript}

		place .right.lower.lblScript -relx 0.15 -rely 0.05
		place .right.lower.script -relx 0.15 -rely 0.27
		place .right.lower.bScript -relx 0.15 -rely 0.55 -width 70 -height 30
	}

	place .right.lblType -relx 0.15 -rely 0.03
	place .right.scrlDisplay -relx 0.81 -rely 0.12 -relheight 0.5 -relwidth 0.04
	place .right.tDisplay -relx 0.15 -rely 0.12 -relheight 0.50 -relwidth 0.7 -anchor nw

	place .right.lower -relx 0.15 -rely 0.75 -relheight 0.2 -relwidth 0.7 -anchor nw

	::persistentWidgets::configStatusbar "Ready"
}


####################################################################
## Procedure Name: iterationComplete
## Precondition: an iterationComplete message must exist
## Postcondition: an iterationComplete dialog window is displayed
## Description: procedure takes iteration complete message and displays
##		    it in a dialog window
## Arguments: none
##
#####################################################################

proc ::dynamicWidgets::makeIterationComplete {} {

	::persistentWidgets::configStatusbar "Ready"

	set ::uiValues::iterationResponse [tk_messageBox -parent . -title "PML Iteration Complete"\
		-type yesno -icon info -message "The PML system has detected that \
		$::uiValues::iterComp is complete.\n\n To close iteration click 'Yes',\
		to return to iteration click 'No'"]
	::inputValidator::getIterationResponse
}

####################################################################
## Procedure Name: makeInfo
## Precondition: an info message must exist
## Postcondition: an info dialog window is displayed
## Description: procedure takes an info message and displays it in
##		    a dialog window
## Arguments: none
##
#####################################################################

proc ::dynamicWidgets::makeInfo {} {

	::persistentWidgets::configStatusbar "Ready"

	set infoMsg [format "%s\n%s%s" \
	    [lindex [lindex $::uiValues::infoText 0] 0] \
	    "To return to PML session click Yes, "\
	    "to close PML session click No" ]

	set ::uiValues::infoResponse [tk_messageBox -parent . \
					  -title "PML Information" \
					  -type yesno -icon info \
					  -message $infoMsg]
	::inputValidator::getInfoResponse
}



####################################################################
## Procedure Name: makeError
## Precondition: an error message must exist
## Postcondition: an error dialog window is displayed
## Description: procedure takes an error message and displays it in
##		    a dialog window
## Arguments: none
##
#####################################################################

proc ::dynamicWidgets::makeError {} {

    ::persistentWidgets::configStatusbar "Ready"

    set errMsg [format "%s\n%s%s" \
		     [lindex [lindex $::uiValues::errorText 0] 0] \
		     "To return to PML session click Yes, "\
		     "to close PML session click No" ]

    ::uiErrorHandler::SendMsg $errMsg

    ::inputValidator::getErrorResponse
}

