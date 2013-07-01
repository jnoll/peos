namespace eval ::uiKernelInterface:: {
variable ModuleName "uiKernelInterface.tcl\:\:"
}

proc ::uiKernelInterface::EstablishConnection {} {

  set engineHost thalia.cudenver.edu
  set enginePort 14523
  variable ModuleName 
  set UIEngineSocket [socket $engineHost $enginePort]

#    if {[catch $socketCommand commandResult] != 0} then {
#
# If socket connection cannot be established then with Engine send an
# error message to the Error Handling module

#	::uiErrorHandler::SendMsg \
#	    [format "%s Unable to establish a connection with Engine on Port %d on Host %s" \
#		     $ModuleName  $enginePort $engineHost]
 #      ::uiProcessHandler::uiProcessScreen
  #  } else {

# Configure the socket to call the ::uiKernelInterface::ReplyReady routine
# when a message is ready to read from the engine daemon
#
	fconfigure $UIEngineSocket -buffering none
	fileevent $UIEngineSocket readable [list \
		::uiEngConn::ReplyReady $UIEngineSocket]
#	if {$DebugOn} then {
#	    puts stdout [format "%s Configuring Socket Event Handler" ]
#	}
	uiProcessHandler::uiProcessScreen

 #   }
return $UIEngineSocket
}

proc ::uiKernelInterface::CloseConnection { uiEngSocket } {

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
