#######################################################################
## File Information: $Id: uiValues.tcl,v 1.1 1999/05/06 04:24:54 jneuder Exp $
#######################################################################

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


proc ::uiValues::resetCommonVariables {} {
        # common variables
        variable userName ""
        variable processName "N/A"
}
        
proc ::uiValues::resetProcessSelectVariables {} {
        # process Select variables
        variable resumeList ""
        variable availList ""
        variable processType ""
}

proc ::uiValues::resetChoiceListVariables {} {
        # choice list variables
        variable choiceList ""
        variable choiceType ""
        variable choiceName ""
}

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

proc ::uiValues::resetIterationCompleteVariables {} {
        # iteration complete variables
        variable iterComp ""    
        variable iterResponse ""
        variable iterValue ""
}

proc ::uiValues::resetInfoVariables {} {
        # info message variables
        variable infoText ""
        variable infoResponse ""
}

proc ::uiValues::resetErrorVariables {} {
        # error message variables
        variable errorText ""
        variable errorResponse ""
}

proc ::uiValues::resetOutputVariables {} {
        # message response variables
        variable output ""
}