#######################################################################
## File Information: $Id: persistentWidgets.tcl,v 1.5 2000/04/10 23:07:29 nmodi Exp $
#######################################################################

# persistentWidgets script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sotware Engin.) Spring 1999, Dr. Noll
package provide persistentWidgets 1.0

namespace eval ::persistentWidgets {

}

####################################################################
## Procedure Name: makeLeftFrame
## Precondition: the widget manager has been invoked
## Postcondition: the left side frame and text labels have been created
## Description: a frame is created and accompanying text labels are
##		drawn and placed
## Arguments: none
##
#####################################################################

proc ::persistentWidgets::makeLeftFrame {} {
	# create left frame and all it's persistent widgets
	frame .left -borderwidth 2 -relief groove
	label .left.systemstate -text "System State" -font headerfont
	label .left.process -text "Process:" -font boldfont
	label .left.processname -text $::uiValues::processName
	label .left.task -text "Task:" -font boldfont
	label .left.taskname -text $::uiValues::taskName
	label .left.taskstatelabel -text "Task State:" -font boldfont
	label .left.taskstate -text $::uiValues::taskState
	label .left.action -text "Action:" -font boldfont
	label .left.actionname -text $::uiValues::actionName
	label .left.actionstatelabel -text "Action State:" -font boldfont
	label .left.actionstate -text $::uiValues::actionState
	label .left.steplabel -text Step: -font boldfont
	label .left.actionstep -text "# of N"

	# draw and place each widget on main window
	place .left -relx 0.01 -rely 0.01 -relheight 0.92 \
	       -relwidth 0.2 -anchor nw
	place .left.systemstate -relx 0.04 -rely 0.01
	place .left.process -relx 0.04 -rely 0.1
	place .left.processname -relx 0.04 -rely 0.15
	place .left.task -relx 0.04 -rely 0.23
	place .left.taskname -relx 0.04 -rely 0.28
	place .left.taskstatelabel -relx 0.04 -rely 0.34
	place .left.taskstate -relx 0.04 -rely 0.39
	place .left.action -relx 0.04 -rely 0.47
	place .left.actionname -relx 0.04 -rely 0.52
	place .left.actionstatelabel -relx 0.04 -rely 0.58
	place .left.actionstate -relx 0.04 -rely .63
	place .left.steplabel -relx 0.04 -rely 0.90
	place .left.actionstep -relx 0.04 -rely 0.95
}


####################################################################
## Procedure Name: makeRightFrame
## Precondition: the widget manager has been invoked
## Postcondition: the right side frame has been created
## Description: a frame is created drawn and placed
## Arguments: none
##
#####################################################################

proc  ::persistentWidgets::makeRightFrame {} {

	# create empty right frame
	frame .right -borderwidth 2 -relief groove

	# draw right frame and place it on window
	place .right -relx 0.24 -rely 0.01 -relheight 0.92 -relwidth 0.75 -anchor nw
}


####################################################################
## Procedure Name: redrawRightFrame
## Precondition: right frame exists
## Postcondition: right frame has been cleared and redrawn
## Description: the right frame and all children widgets are first
##		destroyed and then the default right frame is redrawn
## Arguments: none
##
#####################################################################

proc ::persistentWidgets::redrawRightFrame {} {
	destroy .right

	makeRightFrame
}


####################################################################
## Procedure Name: redrawLeftFrame
## Precondition: left frame exists
## Postcondition: left frame has been cleared and redrawn
## Description: the left frame and all children widgets are first
##		destroyed and then the default left frame is redrawn
## Arguments: none
##
#####################################################################

proc ::persistentWidgets::redrawLeftFrame {} {
	destroy .left

	makeLeftFrame
}


####################################################################
## Procedure Name: makeStatusbar
## Precondition: UI has been invoked
## Postcondition: a status bar area and text label have been created
## Description: a status bar is created and placed at bottom of main
##		window
## Arguments: none
##
#####################################################################

proc ::persistentWidgets::makeStatusbar {} {

	# create a statusbar
	label .statusbar -borderwidth 1 -relief groove -anchor w

	# draw statusbar and place at bottom of screen
	place .statusbar -relx 0.01 -rely 0.96 -relheight 0.04 \
	     -relwidth 0.98 -anchor nw
}


####################################################################
## Procedure Name:configProcess
## Precondition:  Process name label exists in left frame
## Postcondition: Process name label is set
## Description: sets the process text string in the left frame
## Arguments: the string to be placed in the text label
##
#####################################################################

proc  ::persistentWidgets::configProcess {string} {
	.left.processname configure -text $string
}



####################################################################
## Procedure Name: configTaskName
## Precondition:  Task name label exists in left frame
## Postcondition: Task name label is set
## Description: sets the task name text string in the left frame
## Arguments: the string to be placed in the text label
##
#####################################################################

proc ::persistentWidgets::configTaskName {string} {
	.left.taskname configure -text $string
}


####################################################################
## Procedure Name: configTaskState
## Precondition:  Task state label exists in left frame
## Postcondition: Task state label is set
## Description: sets the task state text string in the left frame
## Arguments: the string to be placed in the text label
##
#####################################################################

proc ::persistentWidgets::configTaskState {string} {
	.left.taskstate configure -text $string
}


####################################################################
## Procedure Name: configActionName
## Precondition:  Action name label exists in left frame
## Postcondition: Action name label is set
## Description: sets the action name text string in the left frame
## Arguments: the string to be placed in the text label
##
#####################################################################

proc ::persistentWidgets::configActionName {string} {
	.left.actionname configure -text $string
}


####################################################################
## Procedure Name: configActionStep
## Precondition:  Action step label exists in left frame
## Postcondition: Action step label is set
## Description: sets the action step text string in the left frame
## Arguments: the string to be placed in the text label
##
#####################################################################

proc ::persistentWidgets::configActionStep {string} {
	.left.actionstep configure -text $string
}



####################################################################
## Procedure Name: configStatusbar
## Precondition:  Statusbar label exists
## Postcondition: Statusbar label is set
## Description: sets the statusbar text string
## Arguments: the string to be placed in the text label
##
#####################################################################

proc ::persistentWidgets::configStatusbar {string} {
	.statusbar configure -text $string
}



#
# utilities to create menus
#

####################################################################
## Procedure Name: CreateMenuBar
## Precondition: makeMenu has been called
## Postcondition: menubar associated with top level window has been
##		  created
## Description: menu is declared
## Arguments: toplevel window name and name of menu to create
##
#####################################################################

# create a menubar for Tk version >= 8.0
#
# variables
#	toplevel = name of toplevel window
#	menubar = name of menubar to create

proc ::persistentWidgets::CreateMenubar { toplevel menubar } {
	menu $menubar -type menubar
	$toplevel configure -menu $menubar
}



####################################################################
## Procedure Name: CreateMenu
## Precondition: makeMenu has been called
## Postcondition: menu item has been created
## Description: menu item is added to existing menu
## Arguments: parent menubar, name of base menu, text to display and
##	      letter to underline
##
#####################################################################

# create a menu on a menubar for Tk version >= 8.0
#
# variables
#	menubar = name of parent menubar
#	basename = base name of menu
#	text = text to display
#	mnemonic = which letter to underline

proc ::persistentWidgets::CreateMenu { menubar basename text mnemonic } {
	global tk_version menu_count

	# set up menu name
	set menuName "$menubar.$basename"
	$menubar add cascade -label $text -menu $menuName -underline $mnemonic

	menu $menuName
	return $menuName
}


####################################################################
## Procedure Name: makeMenu
## Precondition: UI has been invoked and called fro menu creation
## Postcondition: menu has been created and populated
## Description: creates menu bar and then adds items and associted
##		commands
## Arguments: none
##
#####################################################################

proc ::persistentWidgets::makeMenu {} {
    CreateMenubar . .menubar

# File Menu
    set menuName [CreateMenu .menubar filemenu "File" 0]

    $menuName add command -label "Open" \
	-command {::uiConfig::ConfigPopup Open } -underline 0
    $menuName add command -label "Save" \
	-command {::uiConfig::ConfigPopup Save } -underline 0
    $menuName add command -label "Exit" -command {exit} -underline 1

# Options Menu
    set menuName [CreateMenu .menubar optionMenu "Options" 0]

    $menuName add command -label "Customize" \
	-command {::uiConfig::OptionsPopup} -underline 0
    $menuName add command -label "Login" \
	-command {::persistentWidgets::makeLoginWin} \
	-underline 0

# Test Menu
    if {$::uiConfig::TestMode} then {
	set menuName [CreateMenu .menubar testMenu "Test" 0]

	$menuName add command -label "Process Select" \
	    -command { uiTest::test_process_select } -underline 0
	$menuName add command -label "Choice List - Action" \
	    -command { uiTest::test_choice_list Action } -underline 14
	$menuName add command -label "Choice List - Branch" \
	    -command { uiTest::test_choice_list Branch } -underline 14
	$menuName add command -label "Choice List - Selection" \
	    -command { uiTest::test_choice_list Selection } -underline 14
	$menuName add command -label "Action - Manual" \
	    -command { uiTest::test_action Manual } -underline 9
	$menuName add command -label "Action - Executable" \
	    -command { uiTest::test_action Executable } -underline 10
	$menuName add command -label "Iteration Complete" \
	    -command { uiTest::test_iteration_complete } -underline 0
	$menuName add command -label "Info" \
	    -command { uiTest::test_info } -underline 2
    }

# Help Menu
    set menuName [CreateMenu .menubar help "Help" 0]
    $menuName add command -label "About" \
			    -command {::persistentWidgets::makeAboutWin} -underline 0
}


####################################################################
## Procedure Name: makeLoginWindow
## Precondition: UI has been invoked, connection with engine established
## Postcondition: user name has beed determined
## Description: creates a login window and reads user entry
## Arguments: none
##
#####################################################################

proc ::persistentWidgets::makeLoginWin {} {

    set entryWidth 20

    toplevel .loginWin -class "PML Login"

    frame .loginWin.userFrame
    pack .loginWin.userFrame -expand yes -fill both

    frame .loginWin.loginSep -height 2 -borderwidth 1 -relief sun
    pack .loginWin.loginSep -fill x

    frame .loginWin.loginControls
    pack .loginWin.loginControls -fill x

    label .loginWin.userFrame.userLabel -text "User Name:"
    entry .loginWin.userFrame.userName -width $entryWidth

   button .loginWin.loginControls.buttonOK -text "OK" \
	-command {::inputValidator::getName \
	     [.loginWin.userFrame.userName get] .loginWin}

    button .loginWin.loginControls.buttonCancel \
	-text "Cancel" -command {destroy .loginWin}

    grid .loginWin.userFrame.userLabel -row 0 -col 0
    grid .loginWin.userFrame.userName  -row 0 -col 1

    grid .loginWin.loginControls.buttonOK -row 0 -col 0
    grid .loginWin.loginControls.buttonCancel -row 0 -col 1

   bind .loginWin.userFrame.userName <Return> \
	{::inputValidator::getName \
	     [.loginWin.userFrame.userName get] .loginWin}


    wm title .loginWin "PML Login"
    tkwait window .loginWin
}

####################################################################
## Procedure Name: makeAboutWin
## Precondition: none
## Postcondition: about window created and displayed
## Description: a dialog box is created and text string is attached
## Arguments: none
##
#####################################################################

proc ::persistentWidgets::makeAboutWin {} {

    tk_messageBox -icon info -title "About PML" \
	-message [format "PML Version 0.01"]
}

