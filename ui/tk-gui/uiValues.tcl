#######################################################################
## File Information: $Id: uiValues.tcl,v 1.2 1999/05/06 20:55:37 jneuder Exp $
#######################################################################

# uiValues script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sofware Engin.) Spring 1999, Dr. Noll

namespace eval ::uiValues {
       
        # Message to be sent to Engine
        variable uiMessage ""

        # common variables
        variable userName ""
        variable processName "N/A"
        
        # process Select variables
        variable resumeList ""
        variable availList ""
        variable processType ""

        # choice list variables
        variable choiceList ""
        variable choiceType ""
        
        # action message variables
        variable process "N/A"
        variable taskName "N/A"
        variable taskState "N/A"   
        variable actionName "N/A"
        variable actionState "N/A"
        variable actionType ""
        variable actionDisplay ""
        variable actionScript ""
        variable fieldList ""
        variable actionTool ""
        variable actionRequires ""
        variable actionProvides ""
        variable actionStep ""

        variable fieldType ""

        # actionField is a list, should it be 3 variables
        variable actionField "" 

        # iteration complete variables
        variable iterComp ""    
        variable iterationResponse ""
        variable iterationValue ""

        # info message variables
        variable infoText ""
        variable infoResponse ""

        # error message variables
        variable errorText ""
        variable errorResponse ""

        # ActionResponse variables
        variable responseProcess ""
        variable responseTaskName ""
        variable responseActionName ""
        variable responseState ""
        variable responseFieldList ""

        # message response variables
        variable output ""
}

## the following reset variables are not used in the initial implementation
## where variables are reset in the parser. These are left to be used as
## needed in the future

####################################################################
## Procedure Name: resetCommonVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################

proc ::uiValues::resetCommonVariables {} {
        # common variables
        variable userName ""
        variable processName "N/A"
}

####################################################################
## Procedure Name: resetProcessSelectVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################
     
proc ::uiValues::resetProcessSelectVariables {} {
        # process Select variables
        variable resumeList ""
        variable availList ""
        variable processType ""
}

####################################################################
## Procedure Name: resetChoiceListVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################

proc ::uiValues::resetChoiceListVariables {} {
        # choice list variables
        variable choiceList ""
        variable choiceType ""
        variable choiceName ""
}

####################################################################
## Procedure Name: resetActionVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################

proc ::uiValues::resetActionVariables {} {      
        # action message variables
        variable process "N/A"
        variable taskName "N/A"
        variable taskState "N/A"   
        variable actionName "N/A"
        variable actionState "N/A"
        variable actionType "N/A"
        variable actionDisplay ""
        variable actionScript ""
        variable fieldList ""
        variable actionTool ""
        variable actionRequires ""
        variable actionProvides ""
        variable actionStep ""

        variable fieldType ""

        # actionField is a list, should it be 3 variables
        variable actionField "" 
}

####################################################################
## Procedure Name: resetIterationCompleteVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################

proc ::uiValues::resetIterationCompleteVariables {} {
        # iteration complete variables
        variable iterComp ""    
        variable iterResponse ""
        variable iterValue ""
}

####################################################################
## Procedure Name: resetInfoVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################

proc ::uiValues::resetInfoVariables {} {
        # info message variables
        variable infoText ""
        variable infoResponse ""
}

####################################################################
## Procedure Name: resetErrorVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################

proc ::uiValues::resetErrorVariables {} {
        # error message variables
        variable errorText ""
        variable errorResponse ""
}

####################################################################
## Procedure Name: resetOutputVariables
## Precondition: none
## Postcondition: variables are set to blank - ""
## Description: the procedure sets the variables to ""
## Arguments: none
##
#####################################################################

proc ::uiValues::resetOutputVariables {} {
        # message response variables
        variable output ""
}

