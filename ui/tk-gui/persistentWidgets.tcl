#######################################################################
## File Information: $Id: persistentWidgets.tcl,v 1.1 1999/05/06 04:24:54 jneuder Exp $
#######################################################################

# persistentWidgets script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sotware Engin.) Spring 1999, Dr. Noll
package provide persistentWidgets 1.0

namespace eval ::persistentWidgets {

}

####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
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
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc  ::persistentWidgets::makeRightFrame {} {
        
        # create empty right frame 
        frame .right -borderwidth 2 -relief groove

        # draw right frame and place it on window
        place .right -relx 0.24 -rely 0.01 -relheight 0.92 -relwidth 0.75 -anchor nw
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::redrawRightFrame {} {
        destroy .right

        makeRightFrame
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::redrawLeftFrame {} {
        destroy .left

        makeLeftFrame
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
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
## Precondition:        Process name label exists in left frame 
## Postcondition:       Process name label is set
## Description:
## Arguments:
##
#####################################################################

proc  ::persistentWidgets::configProcess {string} {
        .left.processname configure -text $string
}



####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::configTaskName {string} {
        .left.taskname configure -text $string
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::configTaskState {string} {
        .left.taskstate configure -text $string
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::configActionName {string} {
        .left.actionname configure -text $string
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::configActionStep {string} {
        .left.actionstep configure -text $string
}



####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::configStatusbar {string} {
        .statusbar configure -text $string
}



#
# utilities to create menus
#

####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:       
## Description: 
## Arguments:
##
#####################################################################

# create a menubar for Tk version >= 8.0
#
# variables
#       toplevel = name of toplevel window
#       menubar = name of menubar to create

proc ::persistentWidgets::CreateMenubar { toplevel menubar } {
        menu $menubar -type menubar
        $toplevel configure -menu $menubar
}



####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

# create a menu on a menubar for Tk version >= 8.0
#
# variables
#       menubar = name of parent menubar
#       basename = base name of menu
#       text = text to display
#       mnemonic = which letter to underline

proc ::persistentWidgets::CreateMenu { menubar basename text mnemonic } {
        global tk_version menu_count
        
        # set up menu name
        set menuName "$menubar.$basename"
        $menubar add cascade -label $text -menu $menuName -underline $mnemonic
                
        menu $menuName
        return $menuName
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
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

# Help Menu
    set menuName [CreateMenu .menubar help "Help" 0]
    $menuName add command -label "About" \
                            -command {::persistentWidgets::makeAboutWin} -underline 0
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
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
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::persistentWidgets::makeAboutWin {} {

    tk_messageBox -icon info -title "About PML" \
        -message [format "PML Version 0.01"]
}