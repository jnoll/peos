#######################################################################
## File Information: $Id: mainWin.tcl,v 1.2 1999/05/07 18:53:09 jneuder Exp $
#######################################################################

package provide mainWin 1.0

# widgetMgr script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sotware Engin.) Spring 1999, Dr. Noll

namespace eval ::mainWin {

}

####################################################################
## Procedure Name:
## Precondition:
## Postcondition:
## Description: Creates main window, sets dimensions, centers on screen
## Arguments:
##
#####################################################################

proc ::mainWin::windowGeometry {} {
	set width [winfo screenwidth .]
	set height [winfo screenheight .]

	set winwidth 600
	set winheight 450

	set x [expr (($width - $winwidth)/2)]
	set y [expr (($height -$winheight)/2)]

	wm geometry . 600x450
	wm geometry . +$x+$y
	wm title . PML

}



####################################################################
## Procedure Name:
## Precondition:
## Postcondition:
## Description: calls procedures to make and manage UI on screen
## Arguments:
##
#####################################################################


# procedure disabled for testing, runs without being invoked

proc ::mainWin::widgetMgr { } {

	::mainWin::windowGeometry

	# set fonts
	font create boldfont -weight bold -size 12
	font create headerfont -weight bold -size 12
	font create biglabelfont -size 16

	# create persistent widgets
	::persistentWidgets::makeLeftFrame
	::persistentWidgets::makeRightFrame
	::persistentWidgets::makeStatusbar

	# dynamic widgets all generated from menu commands
	::persistentWidgets::makeMenu

	# indicate system status
	::persistentWidgets::configStatusbar "Ready"
}
