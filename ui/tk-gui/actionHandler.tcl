#######################################################################
## File Information: $Id: actionHandler.tcl,v 1.1 1999/05/06 04:24:54 jneuder Exp $
#######################################################################

#######################################################################
# Namespace name: ::actionHandler
# Description: This module handles the processing of executable PML
# actions
#######################################################################
package provide actionHandler 1.0

namespace eval ::actionHandler:: {
       
     variable ModuleName "actionHandler\:\:"

     namespace export ExecuteScript
     namespace export ExecuteTool
     namespace export DisplayURL
}



#######################################################################
# Function name:  ::actionHandler::ExecuteScript
# Precondition:   ::actionHandler namespace must be loaded. A script
#                 and a optional list of arguments must be provided.       
# Postcondition:  Executes the script and returns a 1 if the script
#                 executed successfully and a zero if it did not.
# Description:    Executes the script and returns a 1 if the script
#                 executed successfully and a zero if it did not.
# Arguments:      scriptArray --> conmtains the script text,
#                 script name, script arguments, input file,
#                 output file and whether the script should be run in
#                 background.
#######################################################################
proc ::actionHandler::ExecuteScript { ascript } {

    upvar $ascript scriptArray
    
    variable DebugOn
    variable ModuleName

    if {$scriptArray(Name) == "" } then {
        ::uiErrorHandler::SendMsg [format "%s No script name specified" \
                                       $ModuleName]
        return 0;
    }

    if {$scriptArray(Text) != "" } then {
        
        if {$DebugOn} then {
            puts stdout [format "%s Saving Script Text." \
                             $ModuleName ]
        }

        set scriptFd [open $scriptArray(Name) "w"]
        
        puts $scriptFd $scriptArray(Text)
        
        close $scriptFd

        if {![file executable $scriptArray(Name)]} then {
            file attributes $scriptArray(Name) -permissions 511
        }
    }

    if {![file exists $scriptArray(Name)]} then {
        ::uiErrorHandler::SendMsg [format "%s Script: %s does not exist" \
                                       $ModuleName $scriptArray(Name)]
        return 0;
    }

    if {$DebugOn} then {
        puts stdout [format "%s Assembling Script Arguments" \
                         $ModuleName]
    }

    set scriptCommand [list $scriptArray(Name) ]

    if {$scriptArray(Args) != "" } then {
        set scriptCommand [concat $scriptCommand \
                               $scriptArray(Args)]
    }

    if {$scriptArray(Input) != "" } then {
        set scriptCommand [concat $scriptCommand " < " \
                                $scriptArray(Input)]
    }
        
    if {$scriptArray(Output) != "" } then {
        set scriptCommand [concat $scriptCommand " > " \
                                $scriptArray(Output)]
    }

    if {$scriptArray(Background)} then {
        set scriptCommand [concat $scriptCommand " &"]
    }

    if {$DebugOn} then {
        puts stdout [format "%s Executing %s" \
                         $ModuleName $scriptCommand]
    }

    set aCommand {eval exec $scriptCommand}

    if {[catch $aCommand retStatus] != 0} then {
        ::uiErrorHandler::SendMsg \
            [format "%s An error occurred while executing %s" \
                    $ModuleName $scriptArray(Name)]
    }
 
    if {$DebugOn} then {
        puts stdout [format "%s Finished Executing Script %s" \
                         $ModuleName $scriptArray(Name)]
    }
  
    return $retStatus
}

#######################################################################
# Function name:  ::actionHandler::ExecuteTool
# Precondition:   ::actionHandler namespace must be loaded. A tool
#                 and a optional list of arguments must be provided.       
# Postcondition:  Executes the tool and returns a 1 if the tool
#                 executed successfully and a 0 if it did not.
# Description:    Executes the tool and returns a 1 if the tool
#                 executed successfully and a 0 if it did not.
# Arguments:      toolArray --> contains a tool name,
#                 tool name, tool arguments, input file,
#                 output file and whether the tool should be run in
#                 background.
#######################################################################
proc ::actionHandler::ExecuteTool { atool } {

    upvar $atool toolArray
    
    variable DebugOn
    variable ModuleName

    if {$toolArray(Name) == "" } then {
        ::uiErrorHandler::SendMsg [format "%s No tool specified" \
                                       $ModuleName]
        return 0;
    }

    if {$DebugOn} then {
        puts stdout [format "%s Checking for location of tool: %s" \
                         $ModuleName $toolArray(Name)]
    }

    set aCommand {eval exec which $toolArray(Name)}

    if {[catch $aCommand retStatus] != 0} then {
        ::uiErrorHandler::SendMsg [format "%s Cannot find tool: %s" \
                                       $ModuleName $toolArray(Name)]
        return $retStatus;
    }

    if {$DebugOn} then {
        puts stdout [format "%s Assembling Tool Arguments" \
                         $ModuleName]
    }

    set toolCommand [list $toolArray(Name) ]

    if {$toolArray(Args) != "" } then {
        set toolCommand [concat $toolCommand \
                               $toolArray(Args)]
    }

    if {$toolArray(Input) != "" } then {
        set toolCommand [concat $toolCommand " < " \
                                $toolArray(Input)]
    }
        
    if {$toolArray(Output) != "" } then {
        set toolCommand [concat $toolCommand " > " \
                                $toolArray(Output)]
    }

    if {$toolArray(Background)} then {
        set toolCommand [concat $toolCommand " &"]
    }

    if {$DebugOn} then {
        puts stdout [format "%s Executing %s" \
                         $ModuleName $toolCommand]
    }

    set aCommand {eval exec $toolCommand}

    if {[catch $aCommand retStatus] != 0} then {
        ::uiErrorHandler::SendMsg \
            [format "%s An error occurred while executing %s" \
                    $ModuleName $toolArray(Name)]
    }
 
    if {$DebugOn} then {
        puts stdout [format "%s Finished Executing Tool %s" \
                         $ModuleName $toolArray(Name)]
    }
  
    return $retStatus
}

#######################################################################
# Function name:  ::actionHandler::DisplayURL
# Precondition:   ::actionHandler namespace must be loaded. A URL
#                 must be provided.       
# Postcondition:  Executes the WEB Browser in the background with the 
#                 passed in URL.
# Description:    Executes the WEB Browser in the background with the 
#                 passed in URL.
# Arguments:      aURL --> a URL.
#
#######################################################################
proc ::actionHandler::DisplayURL { aURL } {
    
    variable DebugOn
    variable ModuleName

    if {$aURL == "" } then {
        ::uiErrorHandler::SendMsg [format "%s No URL specified" \
                                       $ModuleName]
        return 0;
    }

    set commandArray(Name) $::uiConfig::UsrCfg(WebBrowser)
    set commandArray(Args) $aURL
    set commandArray(Input) ""
    set commandArray(Output) ""
    set commandArray(Background) 1

    if {$DebugOn} then {
        puts stdout [format "%s Spawning Web Browser: %s" \
                         $ModuleName $commandArray(Name)]
    }

    set retStatus [::actionHandler::ExecuteTool commandArray]

    if {$DebugOn} then {
        puts stdout [format "%s Completed spawning Web Browser" \
                         $ModuleName $commandArray(Name)]
    }
 
    return $retStatus
}

#######################################################################
# Function name:  ::actionHandler::TurnDebugOn
# Precondition:   ::actionHandler namepace must be loaded       
# Postcondition:  variable Debug is Set to 1
# Description:    Turns on Debugging Feature of Module
# Arguments:      None.
#######################################################################
proc ::actionHandler::TurnDebugOn {} {

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
# Function name:  ::actionHandler::TurnDebugOff
# Precondition:   ::actionHandler namepace must be loaded       
# Postcondition:  variable Debug is Set to 0
# Description:    Turns off Debugging Feature of Module
# Arguments:      None.
#######################################################################
proc ::actionHandler::TurnDebugOff {} {

    variable DebugOn
    variable ModuleName
  
    if {$DebugOn} then {
        puts stdout [format "%s Turning Debug Off." \
                         $ModuleName]
    }

    set DebugOn 0

    return 1
}