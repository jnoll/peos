#######################################################################
## File Information: $Id: uiConfig.tcl,v 1.3 1999/05/07 16:03:54 jneuder Exp $
#######################################################################

#######################################################################
# Namespace name: ::uiConfig
# Description: This module contains configuration information about the
# PML User Interface such as the Engine Port and the Engine Host. This
# module has the capability to read parameter from the command-line and
# to load and save configuration files.
#######################################################################
package provide uiConfig 1.0

namespace eval ::uiConfig:: {

     variable UsrCfg
     variable DebugOn 0
     variable TestMode 0
     variable ModuleName "uiConfig\:\:"
     variable HostSet 0
     variable PortSet 0
#
# Attempt to set a default config file
#
     if {![string compare $tcl_platform(platform) "unix"] && \
	 [file exists "$env(PWD)/.pmlrc"]} then {
	 set UsrCfg(ConfigFile) "$env(PWD)/.pmlrc"
     } elseif {[file exists "$env(HOME)/.pmlrc"]} then {
	 set UsrCfg(ConfigFile) "$env(HOME)/.pmlrc"
     } else {
	 set UsrCfg(ConfigFile) ""
     }
#
# Clear the Engine Port and Host Name
#
     set UsrCfg(EngineHost) ""
     set UsrCfg(EnginePort) ""
     set UsrCfg(WebBrowser) "netscape"
#
# List of functions which are accessible outside this
# namespace
#
     namespace export SetEngineHost
     namespace export GetEngineHost
     namespace export SetEnginePort
     namespace export GetEnginePort
     namespace export SetConfigFile
     namespace export GetConfigFile
     namespace export SetWebBrowser
     namespace export GetWebBrowser
     namespace export TurnDebugOn
     namespace export TurnDebugOff
     namespace export Init
     namespace export LoadConfigFile
     namespace export SaveConfigFile
}

#######################################################################
# Function name:  ::uiConfig::TurnDebugOn
# Precondition:	  ::uiConfig namepace must be loaded
# Postcondition:  variable Debug is Set to 1
# Description:	  Turns on Debugging Feature of Module
# Arguments:	  None.
#######################################################################
proc ::uiConfig::TurnDebugOn {} {

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
# Function name:  ::uiConfig::TurnDebugOff
# Precondition:	  ::uiConfig namepace must be loaded
# Postcondition:  variable Debug is Set to 0
# Description:	  Turns off Debugging Feature of Module
# Arguments:	  None.
#######################################################################
proc ::uiConfig::TurnDebugOff {} {

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
# Function name:  ::uiConfig::Init
# Precondition:	  ::uiConfig namepace must be loaded and the uiConfig
#		  module has not yet been initialized.
# Postcondition:  The command-line is parsed for arguments. The config
#		  file is parsed if specified.
# Description:	  Initializes the UsrCfg array with information
#		  provided in the command-line.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::Init {} {

    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
	puts stdout [format "%s Initializing Module" \
			 $ModuleName]
    }

    ::uiConfig::ParseCommandLine
    ::uiConfig::LoadConfigFile

    if {$DebugOn} then {
	puts stdout [format "%s Module Initialization Complete" \
			 $ModuleName]
    }

    return 1
}

#######################################################################
# Function name:  ::uiConfig::PrintCmdLineOptions
# Precondition:	  ::uiConfig module has been loaded and the -h or -help
#		  option has been specified.
# Postcondition:  Prints Command-line arguments and then exits.
# Description:	  Prints Command-line arguments and then exits.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::PrintCmdLineOptions {} {


    puts -nonewline stdout [format "\nUsage: %s " $::argv0]
    puts -nonewline stdout [format "\[-f <config_file>\] \[<engine_host>:<engine_port>\]"]
    puts stdout [format "\n\n"]

    puts -nonewline stdout [format "\nExample: %s " $::argv0]
    puts -nonewline stdout [format "-f \$PWD/pmlcfg oslab-server.cudenver.edu:14175"]
    puts stdout [format "\n\n"]

    puts stdout [format "Description: %s invokes the PML User Interface. " $::argv0]
    puts stdout [format "Arguments on the command-line override any settings in"]
    puts stdout [format "the optionally specified configuration file. If no configuration file "]
    puts stdout [format "is specified the local directory and then the \$HOME directory is searched for"]
    puts stdout [format "a config file of the name .pmlrc, which will be loaded as the default "]
    puts stdout [format "configuration file."]
    puts stdout [format "\n\n"]

    exit
}

#######################################################################
# Function name:  ::uiConfig::SetConfigFile
# Precondition:	  ::uiConfig namespace must be loaded. Configuration
#		  FileName must be set
# Postcondition:  UsrCfg ConfigFile array field is set to passed in
#		  configuartion file.
# Description:	  Checks to see if the config file passed in exists.
#		  If it is set according to the Postcondition above.
# Arguments:	  configFile --> Name of UI Configuration File
#######################################################################
proc ::uiConfig::SetConfigFile {configFile} {

    variable UsrCfg
    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
	puts stdout [format "%s Setting Config File To %s" \
			 $ModuleName $configFile]
    }

    if {[file exists $configFile]} then {
	set UsrCfg(ConfigFile) $configFile
    } else {
	::uiErrorHandler::SendMsg \
	    [format "%s Config File\: %s Does Not Exist" \
		 $ModuleName $configFile]
	return 0
    }

    return 1
}

#######################################################################
# Function name:  ::uiConfig::GetConfigFile
# Precondition:	  ::uiConfig namespace must be loaded. Configuration
#		  FileName must be set
# Postcondition:  Config File name is returned
# Description:	  Returns Config File Name.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::GetConfigFile {} {

    variable UsrCfg

    return $UsrCfg(ConfigFile)
}

#######################################################################
# Function name:  ::uiConfig::SetEngineHost
# Precondition:	  ::uiConfig namespace must be loaded. Valid Engine
#		  Engine Host Name.
# Postcondition:  The EngineHost field of the UsrCfg Array is set to
#		  the host passed in.
# Description:	  The EngineHost field of the UsrCfg Array is set to
#		  the host passed in.
# Arguments:	  engineHost --> String containing the Hostname of the
#		  Engine.
#######################################################################
proc ::uiConfig::SetEngineHost {engineHost} {

    variable UsrCfg
    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
	puts stdout [format "%s Setting Engine Host To %s" \
			 $ModuleName $engineHost]
    }

    set UsrCfg(EngineHost) $engineHost

    return 1
}

#######################################################################
# Function name:  ::uiConfig::GetEngineHost
# Precondition:	  ::uiConfig namespace must be loaded. Valid Engine
#		  Engine Host Name.
# Postcondition:  The host name of the Engine is returned.
# Description:	  Returns the hostname of the Engine Host.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::GetEngineHost {} {

    variable UsrCfg

    return $UsrCfg(EngineHost)
}

#######################################################################
# Function name:  ::uiConfig::SetEnginePort
# Precondition:	  ::uiConfig namespace must be loaded. Valid Engine
#		  Engine Port Number.
# Postcondition:  The EnginePort field of the UsrCfg Array is set to
#		  the port number passed in.
# Description:	  The EnginePort field of the UsrCfg Array is set to
#		  the port number passed in.
# Arguments:	  enginePort --> String containing the Port Number of
#		  the Engine.
#######################################################################
proc ::uiConfig::SetEnginePort {enginePort} {

    variable UsrCfg
    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
	puts stdout [format "%s Setting Engine Port To %s" \
			 $ModuleName $enginePort]
    }

    set UsrCfg(EnginePort) $enginePort

    return 1
}

#######################################################################
# Function name:  ::uiConfig::GetEnginePort
# Precondition:	  ::uiConfig namespace must be loaded. Valid Engine
#		  Engine Port Number
# Postcondition:  The port number of the engine is returned.
# Description:	  Returns the port number of the Engine Host.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::GetEnginePort {} {

    variable UsrCfg

    return $UsrCfg(EnginePort)
}

#######################################################################
# Function name:  ::uiConfig::SetWebBrowser
# Precondition:	  ::uiConfig namespace must be loaded. Valid Web Browser
#		  command must be passed in.
# Postcondition:  The WebBrowser field of the UsrCfg Array is set to
#		  the web browser command passed in.
# Description:	  The WebBrowser field of the UsrCfg Array is set to
#		  the web browser in.
# Arguments:	  webBrowser --> Web Browser Command.
#######################################################################
proc ::uiConfig::SetWebBrowser {webBrowser} {

    variable UsrCfg
    variable DebugOn
    variable ModuleName

    if {$DebugOn} then {
	puts stdout [format "%s Setting Web Browser Command To %s" \
			 $ModuleName $webBrowser]
    }

    set UsrCfg(WebBrowser) $webBrowser

    return 1
}

#######################################################################
# Function name:  ::uiConfig::GetWebBrowser
# Precondition:	  ::uiConfig namespace must be loaded. A valid web
#		  browser must be set in the UsrCfg array.
# Postcondition:  Returns the name of the Web Browser command.
# Description:	  Returns the name of the Web Browser command.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::GetWebBrowser {} {

    variable UsrCfg

    return $UsrCfg(WebBrowser)
}

#######################################################################
# Function name:  ::uiConfig::ParseCommandLine
# Precondition:	  ::uiConfig namespace must be loaded.
# Postcondition:  The UsrCfg array has its fields set based on
#		  parameters provided on the command-line.
# Description:	  Loads arguments passed in on the command-line.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::ParseCommandLine {} {

    variable DebugOn
    variable ModuleName
    variable HostSet
    variable PortSet
    variable TestMode
    variable TempEngineHost ""
    variable TempEnginePort ""
    variable TempConfigFile ""
    variable Temp
    variable Temp2
    variable Temp3

    if {$DebugOn} then {
	puts stdout [format "%s Starting to Parse Command Line" \
			 $ModuleName]
    }

    if {[regexp -nocase {([^ ]+):([0-9]+)} \
	     $::argv Temp TempEngineHost TempEnginePort]} then {
	::uiConfig::SetEngineHost $TempEngineHost
	::uiConfig::SetEnginePort $TempEnginePort
	set HostSet 1
	set PortSet 1
    }
    if {[regexp -nocase {(^|[ ]+)-f[ ]+([^\-][^ ]*)} \
	     $::argv Temp Temp2 TempConfigFile]} then {

	::uiConfig::SetConfigFile $TempConfigFile
    }
    if {[regexp -nocase {(^|[ ]+)-h(elp)?} $::argv]} then {

	::uiConfig::PrintCmdLineOptions
    }

    if {[regexp -nocase {(^|[ ]+)-debug} $::argv]} then {

       ::parser::TurnDebugOn
       ::generator::TurnDebugOn
       ::uiEngConn::TurnDebugOn
       ::uiConfig::TurnDebugOn
    }

    if {[regexp -nocase {(^|[ ]+)-test} $::argv]} then {
       set ::uiConfig::TestMode 1
    }

    if {$DebugOn} then {
	puts stdout [format "%s Completed Command Line Parsing" \
			 $ModuleName]
    }

    return 1
}

#######################################################################
# Function name:  ::uiConfig::LoadConfigFile
# Precondition:	  ::uiConfig namespace must be loaded. A valid config
#		  file name in the UserCfg(ConfigFile) field.
# Postcondition:  The Config file is loaded and if the parameter were
#		  not already set on the command-line they are filled
#		  into the UsrCfg Array from information in the config
#		  file.
# Description:	  The Config file is loaded and if the parameter were
#		  not already set on the command-line they are filled
#		  into the UsrCfg Array from information in the config
#		  file.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::LoadConfigFile {} {

     variable UsrCfg
     variable DebugOn
     variable ModuleName
     variable PortSet
     variable HostSet

     if {$UsrCfg(ConfigFile) == ""} then {
	 puts stdout [format "%s No Config File Specified" \
			  $ModuleName]
	 return 0
     } elseif {![file exists $UsrCfg(ConfigFile)]} then {
	 ::uiErrorHandler::SendMsg \
	     [format "%s Config File\: %s Does Not Exist" \
		  $ModuleName $configFile]
	 return 0
     }

     if {$DebugOn} then {
	 puts stdout [format "%s Loading Config File %s" \
			  $ModuleName $UsrCfg(ConfigFile)]
     }

     set configFd [open $UsrCfg(ConfigFile) r]

     while {[gets $configFd aCfgLine] >= 0} {

	 if {[regexp {([^ ]+):[ \x9]+([^ ]+)} \
		  $aCfgLine Temp elementName elementValue]} then {

	     if {![string compare $elementName "EnginePort"]  && \
		      $PortSet || \
		     ![string compare $elementName "EngineHost"] \
		     && $HostSet} then {
		 continue
	     }

	     set UsrCfg($elementName) $elementValue
	 }
     }

     set HostSet 0
     set PortSet 0

     close $configFd

     if {$DebugOn} then {
	 puts stdout [format "%s Completed Loading Config File %s" \
			  $ModuleName $UsrCfg(ConfigFile)]
     }

     return 1
}

#######################################################################
# Function name:  ::uiConfig::SaveConfigFile
# Precondition:	  ::uiConfig namespace must be loaded. A valid config
#		  file name must be passed in.
# Postcondition:  Information in the UsrCfg array is saved into the
#		  Configuration File specified as an input parameter.
# Description:	  Information in the UsrCfg array is saved into the
#		  Configuration File specified as an input parameter.
# Arguments:	  configFile --> valid configuration filename.
#######################################################################
proc ::uiConfig::SaveConfigFile {configFile} {

     variable UsrCfg
     variable DebugOn
     variable ModuleName

     if {$DebugOn} then {
	 puts stdout [format "%s Saving Config File %s" \
			  $ModuleName $configFile]
     }

     set configFd [open $configFile w]
     set configId [array startsearch UsrCfg]

     while {[array anymore UsrCfg $configId]} {

	 set usrField [array nextelement UsrCfg $configId]
	 puts $configFd "$usrField\: $UsrCfg($usrField)"
     }

     array donesearch UsrCfg $configId

     close $configFd

     if {$DebugOn} then {
	 puts stdout [format "%s Completed Saving Config File %s" \
			  $ModuleName $UsrCfg(ConfigFile)]
     }

     return 1
}

#######################################################################
# Function name:  ::uiConfig::ConfigPopup
# Precondition:	  ::uiConfig namespace must be loaded. A valid mode
#		  must be passed in (either "Open" or "Save")
# Postcondition:  A configuration file is opened or saved depending
#		  on the passed in mode.
# Description:	  Displays the file dialog boxes for opening and saving
#		  configuration files.
# Arguments:	  mode --> one of two modes: "Open" or "Save"
#######################################################################
proc ::uiConfig::ConfigPopup {{ mode Open }} {

     variable UsrCfg
     variable DebugOn
     variable ModuleName

     if {$DebugOn} then {
	 puts stdout [format "%s Displaying %s Config Popup" \
			  $ModuleName $mode]
     }

     set configFileTypes {
	 { "Default Config File" { .cfg} }
	 { "All Files" * }
     }

     set tempFileName ""

     set title [format "PML UI %s Configuration Popup" $mode]

     if {$mode == "Open"} then {

	 set tempFileName [tk_getOpenFile \
			       -initialdir $::env(PWD) \
			       -filetypes $configFileTypes \
			       -title $title \
			       -initialfile ".pmlrc" \
			       -parent .]

	 if {[string compare $tempFileName ""] && \
		 [file exists $tempFileName]} then {
	 set UsrCfg(ConfigFile) $tempFileName
	     ::uiConfig::LoadConfigFile
	 }

     } elseif {$mode == "Save"} then {

	 set tempFileName [tk_getSaveFile \
			       -initialdir $::env(PWD) \
			       -defaultextension ".cfg" \
			       -filetypes $configFileTypes \
			       -title $title  \
			       -initialfile ".pmlrc" \
			       -parent .]

	 if {[string compare $tempFileName ""]} then {
	     ::uiConfig::SaveConfigFile $tempFileName
	 }
     }

     return 1
}

#######################################################################
# Function name:  ::uiConfig::OptionsPopup
# Precondition:	  ::uiConfig namespace must be loaded. Valid options
#		  should be stored in the UsrCfg array.
# Postcondition:  UsrCfg entries are set based off on information
#		  set by the user through the popup.
# Description:	  UsrCfg entries are set based off on information
#		  set by the user through the popup.
# Arguments:	  None.
#######################################################################
proc ::uiConfig::OptionsPopup {} {

     variable UsrCfg
     variable DebugOn
     variable ModuleName
     variable entryWidth 30
     variable fieldPadding 5

# created a nested namespace for callbacks

     namespace	eval ::uiConfig::OptionsPopup::Callbacks {

     }

# reset options to original values when the reset button is pushed

     proc ::uiConfig::OptionsPopup::Callbacks::resetOpts {} {

	 .uiOptions.optFields.webBrowserCommand delete 0 \
	     $::uiConfig::entryWidth
	 .uiOptions.optFields.engineHostName delete 0 \
	     $::uiConfig::entryWidth
	 .uiOptions.optFields.enginePortNumber delete 0 \
	     $::uiConfig::entryWidth

	 .uiOptions.optFields.webBrowserCommand insert 0 \
	     [::uiConfig::GetWebBrowser]
	 .uiOptions.optFields.engineHostName insert 0 \
	     [::uiConfig::GetEngineHost]
	 .uiOptions.optFields.enginePortNumber insert 0 \
	     [::uiConfig::GetEnginePort]

	 return 1
     }

# save settings in the popup into the UsrCfg array when
# the user selects the save option

     proc ::uiConfig::OptionsPopup::Callbacks::saveOpts {} {

	 ::uiConfig::SetEngineHost \
	     [.uiOptions.optFields.engineHostName get]

	 ::uiConfig::SetEnginePort \
	     [.uiOptions.optFields.enginePortNumber get]

	  ::uiConfig::SetWebBrowser \
	     [.uiOptions.optFields.webBrowserCommand get]

	 return 1
     }


     if {$DebugOn} then {
	 puts stdout [format "%s Displaying Options Popup" \
			  $ModuleName]
     }

# Create top level popup window

     toplevel .uiOptions -class "PML Options"

# Create frame where the option field will be placed

     frame .uiOptions.optFields
     pack .uiOptions.optFields -expand yes -fill both \
	 -padx $fieldPadding -pady $fieldPadding

# Create a separator of aesthetics

     frame .uiOptions.optSep -height 2 -borderwidth 1 -relief sunken
     pack .uiOptions.optSep -fill x -pady $fieldPadding

# Create a frame to place buttons

     frame .uiOptions.optControls
     pack .uiOptions.optControls -fill x \
	 -padx $fieldPadding -pady $fieldPadding

# Create Web Browser option field

     label .uiOptions.optFields.webBrowserLabel -text "WEB Browser Command: "
     entry .uiOptions.optFields.webBrowserCommand -width $entryWidth

# Create Engine Host option field

     label .uiOptions.optFields.engineHostLabel -text "Engine Host Name: "
     entry .uiOptions.optFields.engineHostName -width $entryWidth

# Create Engine Port Option field

     label .uiOptions.optFields.enginePortLabel -text "Engine Port Number: "
     entry .uiOptions.optFields.enginePortNumber -width $entryWidth

# Create Save, Reset and Cancel buttons

     button .uiOptions.optControls.saveButton -text "Save" \
	 -command {::uiConfig::OptionsPopup::Callbacks::saveOpts}
     button .uiOptions.optControls.resetButton -text "Reset" \
	 -command {::uiConfig::OptionsPopup::Callbacks::resetOpts}
     button .uiOptions.optControls.cancelButton -text "Cancel" \
	 -command {destroy .uiOptions}

# Place fields and buttons onto popup

     grid .uiOptions.optFields.webBrowserLabel -row 0 -col 0 -sticky e \
	  -padx $fieldPadding -pady $fieldPadding
     grid .uiOptions.optFields.webBrowserCommand -row 0 -col 1 -sticky w \
	  -padx $fieldPadding -pady $fieldPadding

     grid .uiOptions.optFields.engineHostLabel -row 1 -col 0 -sticky e \
	  -padx $fieldPadding -pady $fieldPadding
     grid .uiOptions.optFields.engineHostName -row 1 -col 1 -sticky w \
	  -padx $fieldPadding -pady $fieldPadding

     grid .uiOptions.optFields.enginePortLabel -row 2 -col 0 -sticky e \
	 -padx $fieldPadding -pady $fieldPadding
     grid .uiOptions.optFields.enginePortNumber -row 2 -col 1 -sticky w \
	 -padx $fieldPadding -pady $fieldPadding

     grid .uiOptions.optControls.saveButton -row 0 -col 0 \
	 -padx $fieldPadding -pady $fieldPadding
     grid .uiOptions.optControls.resetButton -row 0 -col 1 \
	 -padx $fieldPadding -pady $fieldPadding
     grid .uiOptions.optControls.cancelButton -row 0 -col 2 \
	 -padx $fieldPadding -pady $fieldPadding

# initalize option entries to current settings

     ::uiConfig::OptionsPopup::Callbacks::resetOpts

# display window title and wait for users input

     wm title .uiOptions "PML UI Options"
     tkwait window .uiOptions

     return 1
 }
