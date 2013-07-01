#######################################################################
## File Information: $Id: generateMesg.tcl,v 1.2 1999/05/07 18:53:09 jneuder Exp $
#######################################################################

##################################################################
# Function name:  generateMesg
# Precondition:	  The UI has collected user input
# Postcondition:  The message is formatted
# Description:	  Formats a message to send to Engine
# Parameters:
#		  * mesg_type - Type of message
##################################################################

package provide generator 1.0

namespace eval ::generator  {
   variable DebugOn 0
   variable ModuleName "generator\:\:"
   namespace export generateMesg
}


proc ::generator::generateMesg { mesg_type }  {

   variable DebugOn
   variable ModuleName

   # provide feedback to user that engine-UI communication is occurring
   ::persistentWidgets::configStatusbar "Awaiting engine response"

   # Capitalize the message type
   set mesg_type [string toupper $mesg_type]

   # Now create different message types
   switch $mesg_type  {

      LOGIN  {

	   if {$DebugOn} then {
	      puts stdout [format "%s Message type is LOGIN" \
			 $ModuleName]
	      puts $::uiValues::userName
	   }

	   ::generator::buildLogin
	   ::uiEngConn::SendMessage $::uiValues::uiMessage
      }

      PROCESSSELECTION	{

	   if {$DebugOn} then {
	      puts stdout [format "%s Message type is PROCESS SELECTION" \
			   $ModuleName]
	      puts $::uiValues::processName
	      puts $::uiValues::processType
	   }

	   ::generator::buildProcessSelection
	   ::uiEngConn::SendMessage $::uiValues::uiMessage
      }

      CHOICESELECTION  {

	   if {$DebugOn} then {
	      puts stdout [format "%s Message type is CHOICE SELECTION" \
			 $ModuleName]
	      puts $::uiValues::processName
	   }

	   ::generator::buildChoiceSelection
	   ::uiEngConn::SendMessage $::uiValues::uiMessage
      }

      ACTIONRESPONSE  {

	   if {$DebugOn} then {
	      puts stdout [format "%s Message type is ACTION RESPONSE" \
			   $ModuleName]
	      puts $::uiValues::responseProcess
	      puts $::uiValues::responseTaskName
	      puts $::uiValues::responseActionName
	      puts $::uiValues::responseState
	      puts $::uiValues::responseFieldList
	   }

	   ::generator::buildActionResponse
	   ::uiEngConn::SendMessage $::uiValues::uiMessage
      }

      ITERATIONRESPONSE	 {

	   if {$DebugOn} then {
	      puts stdout [format "%s Message type is ITERATION RESPONSE" \
			   $ModuleName]
	      puts $::uiValues::iterationValue
	   }

	   ::generator::buildIterationResponse
	   ::uiEngConn::SendMessage $::uiValues::uiMessage
      }

      default  {
	  ::uiErrorHandler::SendMsg \
	      [format "%s Unrecognized message type %s" \
		   $ModuleName $mesg_type ]
      }

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

