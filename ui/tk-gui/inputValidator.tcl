#######################################################################
## File Information: $Id: inputValidator.tcl,v 1.1 1999/05/06 04:24:54 jneuder Exp $
#######################################################################

# inputValidator script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sofware Engin.) Spring 1999, Dr. Noll

package provide inputValidator 1.0

namespace eval ::inputValidator {

}
        
####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::inputValidator::checkListbox {lbox} {
    set indx [$lbox curselection]
    if {$indx != ""} {
        set item [$lbox get $indx]
        return $item
    } else {
        return ""
    }
}

####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::inputValidator::getChoiceSelected {lbox} {
    set select [checkListbox $lbox]
    
    if {$select != ""} {
	set ::uiValues::choiceName $select
        ::generator::generateMesg "CHOICESELECTION"
    } else {
        # nothing selected, do nothing
    }
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::inputValidator::getProcessSelected {lbox1 lbox2} {
    set select [checkListbox $lbox1]
    
    if {$select != ""} {
        ::persistentWidgets::configProcess $select
        set ::uiValues::processType "RESUMABLE"
        set ::uiValues::processName $select
        ::generator::generateMesg "PROCESSSELECTION"
    }
    
    set select [checkListbox $lbox2]
    
    if {$select != ""} {
        ::persistentWidgets::configProcess $select
        set ::uiValues::processType "AVAILABLE"
        set ::uiValues::processName $select
        ::generator::generateMesg "PROCESSSELECTION"
    }
    
    if {$select == ""} {
        # nothing selected, do nothing
    }
    
}

####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::inputValidator::getRadioButtonResponse {newActionState} {
    ::generator::generateMesg "ACTIONRESPONSE"
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
## Arguments:
##
#####################################################################

proc ::inputValidator::getManualEntry {}  {
    set entryField [.right.lower.entField get]
    ::generator::generateMesg "ACTIONRESPONSE"
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
##
#####################################################################

proc ::inputValidator::getExecScript {} {
    # need some code here
    ::generator::generateMesg "ACTIONRESPONSE"
}

####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
##
#####################################################################

proc ::inputValidator::getIterationResponse {} {
    ::generator::generateMesg "ITERATIONRESPONSE"
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
##
#####################################################################

proc ::inputValidator::getInfoResponse {} {
    
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
##
#####################################################################

proc ::inputValidator::getErrorResponse {} {
                
}


####################################################################
## Procedure Name: 
## Precondition:
## Postcondition:
## Description:
##
#####################################################################

proc ::inputValidator::getName { userName win } {
       
    if {$userName != ""} {
        set ::uiValues::userName $userName
        ::generator::generateMesg "LOGIN"
        destroy $win
        return 1
    } else {
        return 0
    }
}
