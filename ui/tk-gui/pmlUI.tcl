#!/bin/sh
#######################################################################
## File Information: $Id: pmlUI.tcl,v 1.7 2000/05/09 01:59:43 nmodi Exp $
#######################################################################

#
#  Main PML User Interface Script
#

# \
exec wish $0 "$@"

# Source all necessary files
source actionHandler.tcl
source buildMesg.tcl
source dynamicWidgets.tcl
source extractName.tcl
source generateMesg.tcl
source inputValidator.tcl
source mainWin.tcl
source messageMgr.tcl
source parseMesg.tcl
source persistentWidgets.tcl
source readMesgField.tcl
source separateIdName.tcl
source uiConfig.tcl
source uiKernelInterface.tcl
source uiErrorHandler.tcl
source uiValues.tcl
source uiLogin.tcl
source uiHelp.tcl
source uiInputValidator.tcl
source uiProcessHandler.tcl
source uiActionHandler.tcl

# Perform Some Initialization
# ::uiConfig::Init
# ::mainWin::widgetMgr


# Show Login Window After Initialization
#if {$::uiConfig::TestMode} then {
#  source uiTest.tcl
#} else {
#  after idle {
 #    persistentWidgets::makeLoginWin
     uiInputValidator::uiLoginScreen
#  }
#}
