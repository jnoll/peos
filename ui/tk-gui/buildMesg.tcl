#######################################################################
## File Information: $Id: buildMesg.tcl,v 1.2 1999/05/07 18:53:09 jneuder Exp $
#######################################################################

package provide generator 1.0

namespace eval ::generator  {
   variable DebugOn 0
   variable ModuleName "generator\:\:"
   namespace export buildLogin
   namespace export buildProcessSelection
   namespace export buildChoiceSelection
   namespace export buildActionResponse
   namespace export buildIterationResponse
}


##################################################################
# Function name:  buildLogin
# Precondition:	  User has provided necessary login info
# Postcondition:  The Login message is formatted
# Description:	  Formats a message to send to Engine
# Parameters:	  None.
##################################################################

proc ::generator::buildLogin {}	 {

    variable DebugOn
    variable ModuleName

    set ::uiValues::uiMessage [format "LOGIN|USERNAME:%s" $::uiValues::userName]

    if {$DebugOn} then {
	puts stdout [format "%s Message type is LOGIN" \
		$ModuleName]
	puts $::uiValues::uiMessage
    }

}


##################################################################
# Function name:  buildProcessSelection
# Precondition:	  User has selected a process
# Postcondition:  The ProcessSelection message is formatted
# Description:	  Formats a message to send to Engine
# Parameters:	  None.
##################################################################

proc ::generator::buildProcessSelection {}  {

    variable DebugOn
    variable ModuleName

    set ::uiValues::uiMessage [format "PROCESSSELECTION|USERNAME:%s|NAME:%s|TYPE:%s" \
			   $::uiValues::userName  $::uiValues::processName  $::uiValues::processType]

    if {$DebugOn} then {
	puts stdout [format "%s Message type is PROCESSSELECTION" \
		$ModuleName]
	puts $::uiValues::uiMessage
    }

}


##################################################################
# Function name:  buildChoiceSelection
# Precondition:	  User has selected a task or action
# Postcondition:  The ChoiceSelection message is formatted
# Description:	  Formats a message to send to Engine
# Parameters:	  None.
##################################################################

proc ::generator::buildChoiceSelection {}  {

    variable DebugOn
    variable ModuleName

    # Extract the name & id within the choiceName field
    set choiceNameString [extractNameId $::uiValues::choiceName]

    set ::uiValues::uiMessage \
	[format "CHOICESELECTION|USERNAME:%s|PROCESSNAME:%s|NAME:%s" \
	 $::uiValues::userName	$::uiValues::processName  $choiceNameString]

    if {$DebugOn} then {
	puts stdout [format "%s Message type is CHOICESELECTION" \
		$ModuleName]
	puts $::uiValues::uiMessage
    }
}


##################################################################
# Function name:  buildActionResponse
# Precondition:	  User has selected a task or action
# Postcondition:  The ActionResponse message is formatted
# Description:	  Formats a message to send to Engine
# Parameters:	  None.
##################################################################

proc ::generator::buildActionResponse {}  {

    variable DebugOn
    variable ModuleName

    # Build the Field list
    set fieldListLength [llength $::uiValues::responseFieldList]
    set fieldList ""
    for {set i 0} {$i < $fieldListLength} {incr i}  {
	set item [lindex  $::uiValues::responseFieldList  $i]
	set fieldList [format "%s%s,"  $fieldList  $item]
    }
    # Remove the last comma at the end of list
    set strlength [string length $fieldList]
    set fieldList [string range $fieldList  0 [expr $strlength - 2]]

    # Extract the name & id from the Process, TaskName, and ActionName fields
    set processString [extractNameId $::uiValues::responseProcess]
    set taskNameString [extractNameId $::uiValues::responseTaskName]
    set actionNameString [extractNameId $::uiValues::responseActionName]

    # Assemble message
    set ::uiValues::uiMessage \
	[format "ACTIONRESPONSE|USERNAME:%s|PROCESSNAME:%s|PROCESS:%s|TASKNAME:%s|ACTIONNAME:%s|STATE:%s|FIELD:%s" \
	    $::uiValues::userName  $::uiValues::processName  $processString  $taskNameString  \
	    $actionNameString  $::uiValues::responseState  $fieldList]

    if {$DebugOn} then {
	puts stdout [format "%s Message type is ACTIONRESPONSE" \
		$ModuleName]
	puts $::uiValues::uiMessage
    }
}


##################################################################
# Function name:  buildIterationResponse
# Precondition:	  User has responded to the Iteration Complete question
# Postcondition:  The IterationResponse message is formatted
# Description:	  Formats a IterationResponse message to send to Engine
# Parameters:	  None.
##################################################################

proc ::generator::buildIterationResponse {}  {

    variable DebugOn
    variable ModuleName

    set ::uiValues::uiMessage [format "ITERATIONRESPONSE|USERNAME:%s|PROCESSNAME:%s|VALUE:%s"  \
			   $::uiValues::userName  $::uiValues::processName  $::uiValues::iterationValue]

    if {$DebugOn} then {
	puts stdout [format "%s Message type is ITERATIONRESPONSE" \
		$ModuleName]
	puts $::uiValues::uiMessage
    }
}


#######################################################################
# Function name:  ::generator::TurnDebugOn
# Precondition:	  ::generator namepace must be loaded
# Postcondition:  variable Debug is Set to 1
# Description:	  Turns on Debugging Feature of Module
# Arguments:	  None.
#######################################################################
proc ::generator::TurnDebugOn {} {

    variable DebugOn
    variable ModuleName

    set DebugOn 1

    if {$DebugOn} then {
	puts stdout [format "%s Turning Debug On" \
			 $ModuleName]
    }

    return 1
}

#######################################################################
# Function name:  ::generator::TurnDebugOff
# Precondition:	  ::generator namepace must be loaded
# Postcondition:  variable Debug is Set to 0
# Description:	  Turns off Debugging Feature of Module
# Arguments:	  None.
#######################################################################
proc ::generator::TurnDebugOff {} {

    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
	puts stdout [format "%s Turning Debug Off." \
			 $ModuleName]
    }

    set DebugOn 0

    return 1
}


