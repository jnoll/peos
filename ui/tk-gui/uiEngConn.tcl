#######################################################################
## File Information: $Id: uiEngConn.tcl,v 1.1 1999/05/06 03:59:50 jneuder Exp $
#######################################################################

#######################################################################
# Namespace name: ::uiEngConn
# Description: This module handles the connection between the PML 
# Engine and the PML User Interface. There are three routine which are
# exported for use by other PML User Interface Modules. SendMessage,
# TurnDebugOn and TurnDebugOff
#######################################################################

package provide uiEngConn 1.0

namespace eval ::uiEngConn:: {
#
# Variable Common Within the uiEngConn Namespace Area
#       
     variable DebugOn 0
     variable ModuleName "uiEngConn\:\:"
#
# Export the routines so that they can be accessible outside the
# namespace
#
     namespace export TurnDebugOn
     namespace export TurnDebugOff
     namespace export SendMessage
}

#######################################################################
# Function name:  ::uiEngConn::SendMessage
# Precondition:   ::uiEngConn namepace must be loaded and a valid 
#                 UI to Engine Message must be passed in to the routine.
# Postcondition:  engineMessage is sent to the engine daemon via a
#                 socket connection.
# Description:    Sends the message specified by engineMessage to the
#                 engine daemon.
# Arguments:      engineMessage -->  valid UI to Engine Message is 
#                                    character string form.
#######################################################################
proc ::uiEngConn::SendMessage { engineMessage } {

    variable DebugOn
    variable ModuleName
#
# Establish a connection with the engine
#
    set UIEngineSocket [::uiEngConn::EstablishConnection \
                            [::uiConfig::GetEnginePort]  \
                            [::uiConfig::GetEngineHost]]

    if {$DebugOn} then {
        puts stdout [format "%s Sending Message %s to Engine." \
                        $ModuleName $engineMessage]
    }
#
# Send Message to the engine through the socket channel $UIEngineSocket
#     
    puts $UIEngineSocket $engineMessage
    flush $UIEngineSocket

    if {$DebugOn} then {
        puts stdout [format "%s Message %s sent to Engine." \
                         $ModuleName $engineMessage]
    }

    if {$DebugOn} then {
        puts stdout [format "%s Waiting for Reply from Engine." \
                     $ModuleName]
    }

    return 1
}

#######################################################################
# Function name:  ::uiEngConn::TurnDebugOn
# Precondition:   ::uiEngConn namepace must be loaded       
# Postcondition:  variable Debug is Set to 1
# Description:    Turns on Debugging Feature of Module
# Arguments:      None.
#######################################################################
proc ::uiEngConn::TurnDebugOn {} {

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
# Function name:  ::uiEngConn::TurnDebugOff
# Precondition:   ::uiEngConn namepace must be loaded       
# Postcondition:  variable Debug is Set to 0
# Description:    Turns off Debugging Feature of Module
# Arguments:      None.
#######################################################################
proc ::uiEngConn::TurnDebugOff {} {

    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
        puts stdout [format "%s Turning Debug Off." \
                         $ModuleName]
    }

    set DebugOn 0

    return 1
}

#######################################################################
# Function name:  ::uiEngConn::ReplyReady
# Precondition:   ::uiEngConn namepace must be loaded and an active
#                 UI to Engine Socket ready with and Engine reply
#                 message.       
# Postcondition:  Reply is consumed and the socket connection is closed.
# Description:    This routine is called by the socket event handler 
#                 when a reply message is ready on the sosket. The
#                 Message is consumed and the socket connection is 
#                 closed.
# Arguments:      uiEngSocket --> a valid UI to Engine Socket Message
#######################################################################
proc ::uiEngConn::ReplyReady { uiEngSocket } {

    variable EngineMessage ""
    variable DebugOn
    variable ModuleName
 
    if {$DebugOn} then {
        puts stdout [format "%s Reply ready from Engine." \
                     $ModuleName]
    }
#
# Retrieve Engine Message from the socket.
#    
    set EngineMessage [::uiEngConn::ConsumeEngineReply $uiEngSocket]
#
# Close Connection to the Engine
#
    ::uiEngConn::CloseConnection $uiEngSocket
#
# Send Engine Reply Message to the parser
#
    ::parser::parseMesg $EngineMessage

    return 1
}

#######################################################################
# Function name:  ::uiEngConn::ConsumeEngineReply
# Precondition:   ::uiEngConn namepace must be loaded and an active
#                 socket channel must be passed in.       
# Postcondition:  Reply Message is returned from Engine.
# Description:    Using the active passed in socket, ::uiEngConn::
#                 ConsumeEngineReply retrieves the message from the 
#                 socket and returns the message text.
# Arguments:      uiEngSocket -> Active UI/Engine Socket.
#######################################################################
proc ::uiEngConn::ConsumeEngineReply { uiEngSocket } {

    variable DebugOn
    variable ModuleName
     
    if {$DebugOn} then {
        puts stdout [format "%s Consuming Message from Engine." \
                         $ModuleName]
    }

    set TempMessage ""
    set UIMessage ""
#
# While not end of file on the socket and the incoming message is not
# empty then retrieve the Engine Message text from the socket and 
# place it in the UIMessage variable.
#
    while {[gets $uiEngSocket TempMessage] >= 0} {
        if {$TempMessage != ""} then {
            set UIMessage ${UIMessage}${TempMessage}
            if {$DebugOn} then {
                puts stdout [format "%s Creating Message from Engine: %s." \
                                 $ModuleName $UIMessage]
            }
        } else {
            if {$DebugOn} then {
                puts stdout [format "%s Blank Line Encountered from Engine." \
                                 $ModuleName]
            }
            break
        }
    }

    if {$DebugOn} then {
        puts stdout [format "%s Complete Engine Message: %s." \
                     $ModuleName $UIMessage]
    }
#
# Return Message through procedure
#
    return $UIMessage
}

#######################################################################
# Function name:  ::uiEngConn::EstablishConnection
# Precondition:   ::uiEngConn namepace must be loaded. The port number
#                 and the host of the engine daemon.
# Postcondition:  Connection is established with the Engine on a 
#                 socket channel.  The socket event handlers are 
#                 configured to call ::uiEngConn::ReplyReady procedure 
#                 when a message is ready from the Engine. The active
#                 socket is returned.
# Description:    Establishes socket connection with the PML Engine
#                 daemon and configures the socket event handlers.
# Arguments:      enginePort -> port number of engine daemon.
#                 engineHost -> hostname of the engine daemon.
#######################################################################
proc ::uiEngConn::EstablishConnection { enginePort engineHost } {

    variable DebugOn
    variable ModuleName
    variable UIEngineSocket -1

    if {$DebugOn} then {
        puts stdout [format "%s Establishing Engine Connection." \
                            $ModuleName]
    }
#
# Attempt to establish a socket connection
#
    set socketCommand {set UIEngineSocket [socket $engineHost $enginePort]}

    if {[catch $socketCommand commandResult] != 0} then {
#
# If socket connection cannot be established then with Engine send an
# error message to the Error Handling module
#
        ::uiErrorHandler::SendMsg \
            [format "%s Unable to establish a connection with Engine on Port %d on Host %s" \
                    $ModuleName $enginePort $engineHost]
    } else {
#
# Configure the socket to call the ::uiEngConn::ReplyReady routine when
# a message is ready to read from the engine daemon
#
        fconfigure $UIEngineSocket -buffering none   
        fileevent $UIEngineSocket readable [list ::uiEngConn::ReplyReady \
                                                $UIEngineSocket]
        if {$DebugOn} then {
            puts stdout [format "%s Configuring Socket Event Handler" \
                        $ModuleName]
        }
    }

    return $UIEngineSocket
}

#######################################################################
# Function name:  ::uiEngConn::CloseConnection
# Precondition:   ::uiEngConn namepace must be loaded. Active UI to 
#                 Engine socket connection.
# Postcondition:  Socket connection is closed.S
# Description:    Closes connection with the engine daemon.
# Arguments:      uiEngSocket --> Active UI to Engine Socket
#######################################################################
proc ::uiEngConn::CloseConnection { uiEngSocket } {
   
    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
        puts stdout [format "%s Closing Engine Connection." \
                     $ModuleName]
    }
#
# Close Socket Connection.
#
    close $uiEngSocket

    if {$DebugOn} then {
        puts stdout [format "%s Connection Closed." \
                     $ModuleName]
    }

    return 1
}
