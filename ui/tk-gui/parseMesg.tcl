#######################################################################
## File Information: $Id: parseMesg.tcl,v 1.3 1999/05/06 16:44:37 jneuder Exp $
#######################################################################

##################################################################
# Function name:  parseMesg
# Precondition:   A message from UI is read from the pipe
# Postcondition:  The message is parsed into a Tcl array with
#                 each element containing a message field.
# Description:    Parses a message into message types
#                 Returns a specific message structure
# Parameters:
#                 *  message     -  message to be parsed
##################################################################


package provide parser 1.0

namespace eval ::parser  {
   variable DebugOn 0
   variable ModuleName "parser\:\:"
   namespace export parseMesg
}


proc ::parser::parseMesg { message }  {

   variable DebugOn
   variable ModuleName
    
   # Find the end of the first word (delimeter is '|')
   set endpos [string first "|" $message]

   # The first word in the message is the message type
   set mesg_type [string range $message 0 [expr $endpos - 1]]

   # Capitalize the message type
   set mesg_type [string toupper $mesg_type]

   # User name & Process name are available in all messages
   set tempUserName  [readMesgField $message "USERNAME"]
   if {$tempUserName != ""} then {
      set ::uiValues::userName $tempUserName
   }

   set tempProcessName  [readMesgField $message "PROCESSNAME"]
   if {$tempProcessName != ""} then {
      set ::uiValues::processName $tempProcessName 
   }

   # Now create different message types
   switch $mesg_type  {

      PROCESSLIST  {
	   set ::uiValues::resumeList   [readMesgField $message "RESUMABLE"]
	   set ::uiValues::availList   [readMesgField $message "AVAILABLE"]

           if {$DebugOn} then {
              puts stdout [format "%s Message type is PROCESSLIST" \
                          $ModuleName]
              puts $::uiValues::resumeList
	      puts $::uiValues::availList         
	   }

	   ::messageMgr::configUI processList
      }

      CHOICELIST  {
	   set ::uiValues::choiceList  [readMesgField $message "NAMES"]
	   set ::uiValues::choiceType  [readMesgField $message "TYPE"]

           if {$DebugOn} then {
              puts stdout [format "%s Message type is CHOICELIST" \
                           $ModuleName]
              puts $::uiValues::choiceList
              puts $::uiValues::choiceType
           }

	   ::messageMgr::configUI choiceList
      }

	ACTION  {
	   set ::uiValues::process        [readMesgField $message "PROCESS"]
	   set ::uiValues::taskName       [readMesgField $message "TASKNAME"]
	   set ::uiValues::taskState      [readMesgField $message "TASKSTATE"]
	   set ::uiValues::actioName      [readMesgField $message "ACTIONNAME"]
	   set ::uiValues::actionState    [readMesgField $message "STATE"]
	   set ::uiValues::actionType     [readMesgField $message "TYPE"]
	   set ::uiValues::actionDisplay  [readMesgField $message "DISPLAY"]
	   set ::uiValues::actionScript   [readMesgField $message "SCRIPT"]
	   set ::uiValues::fieldList      [readMesgField $message "FIELD"]
	   set ::uiValues::actionTool     [readMesgField $message "TOOL"]
  	   set ::uiValues::actionRequires [readMesgField $message "REQUIRES"]
	   set ::uiValues::actionProvides [readMesgField $message "PROVIDES"]
	   set ::uiValues::actionStep    [readMesgField $message "STEPS"]
	             
           if {$DebugOn} then {
              puts stdout [format "%s Message type is ACTION" \
                           $ModuleName]
              puts $::uiValues::process
              puts $::uiValues::taskName	 
              puts $::uiValues::taskState	 
              puts $::uiValues::actioName	 
              puts $::uiValues::actionState	 
              puts $::uiValues::actionType	 
              puts $::uiValues::actionDisplay	 
              puts $::uiValues::actionScript	 
              puts $::uiValues::fieldList	 
              puts $::uiValues::actionTool	 
              puts $::uiValues::actionRequires	 
              puts $::uiValues::actionProvides	 
              puts $::uiValues::actionStep	 
          }

	  ::messageMgr::configUI action
      }
	 
      ITERATIONCOMPLETE  {
	   set ::uiValues::iterComp [readMesgField $message "NAME"]

           if {$DebugOn} then {
              puts stdout [format "%s Message type is ITERATIONCOMPLETE" \
                           $ModuleName]
              puts $::uiValues::iterComp
           }

	   ::messageMgr::configUI iterationComplete
      }

      INFO  {
	   set ::uiValues::infoText [readMesgField $message "TEXT"]

           if {$DebugOn} then {
              puts stdout [format "%s Message type is INFO" \
                           $ModuleName]
              puts $::uiValues::infoText
           }

	   ::messageMgr::configUI info      
      }
    
      ERROR  {
           set ::uiValues::errorText   [readMesgField $message "TEXT"]

           if {$DebugOn} then {
              puts stdout [format "%s Message type is ERROR" \
                           $ModuleName]
              puts $::uiValues::errorText
           }

	   ::messageMgr::configUI error
      }

      default  {
          ::uiErrorHandler::SendMsg \
              [format "%s Unrecognized message type %s" \
                   $ModuleName $mesg_type ]
      }

   }
}

#######################################################################
# Function name:  ::parser::TurnDebugOn
# Precondition:   ::parser namepace must be loaded       
# Postcondition:  variable Debug is Set to 1
# Description:    Turns on Debugging Feature of Module
# Arguments:      None.
#######################################################################
proc ::parser::TurnDebugOn {} {

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
# Function name:  ::parser::TurnDebugOff
# Precondition:   ::parser namepace must be loaded       
# Postcondition:  variable Debug is Set to 0
# Description:    Turns off Debugging Feature of Module
# Arguments:      None.
#######################################################################
proc ::parser::TurnDebugOff {} {

    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
        puts stdout [format "%s Turning Debug Off." \
                         $ModuleName]
    }

    set DebugOn 0

    return 1
}
