#######################################################################
## File Information: $Id: inputValidator.tcl,v 1.2 1999/05/06 20:55:37 jneuder Exp $
#######################################################################

# inputValidator script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sofware Engin.) Spring 1999, Dr. Noll

package provide inputValidator 1.0

namespace eval ::inputValidator {

}
        
####################################################################
## Procedure Name: checkListbox
## Precondition: user has triggered a selection from a listbox
## Postcondition: the selected item has been returned
## Description: the procedure reads in the selected item, if no
##              selection has been made an empty string is returned
## Arguments: a listbox from which a choe is made
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
## Procedure Name: getChoiceSelected
## Precondition: a choiceList selection has been made by user
## Postcondition: item selected has been determined
## Description: the procedure determines the selected item and then
##              sets a system state label and finally calls the output
##              message generator
## Arguments: the listbox containing items to select from
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
## Procedure Name: getProcessSelected
## Precondition: user has selected the 'Open' button in the
##               processSelect screen
## Postcondition: the selected item has been determined and the
##                appropriate function call has been made to handle
##                user response
## Description: each listbox is checked for a selected item, if one
##              exists the processType and processNames variables
##              are set in uiValues and the appropriate function
##              call is made to the generator
## Arguments: the two listboxes to be checked for a selected item
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
## Procedure Name: getRadioButtonResponse
## Precondition: user has updated the action state
## Postcondition: the engine has been notified of the update
## Description: the procedure calls teh generator function to notify
##              the engine of the action response message
## Arguments: new state for action
##
#####################################################################

proc ::inputValidator::getRadioButtonResponse {newActionState} {
    ::generator::generateMesg "ACTIONRESPONSE"
}


####################################################################
## Procedure Name: getManualEntry
## Precondition: user has entered data into the entry field
## Postcondition: the entered field data has been sent to the engine
## Description: the entry data is determined and the generator is called
##              to notify the engine of the data entered
## Arguments: none
##
#####################################################################

proc ::inputValidator::getManualEntry {}  {
    set entryField [.right.lower.entField get]
    ::generator::generateMesg "ACTIONRESPONSE"
}


####################################################################
## Procedure Name: getExecScript
## Precondition: user has clicked the 'Execute' button to run the
##               associated script **** not yet fully implemented ****
## Postcondition: engine notified of script execution
## Description: N/A
##
#####################################################################

proc ::inputValidator::getExecScript {} {
    # need some code here
    ::generator::generateMesg "ACTIONRESPONSE"
}

####################################################################
## Procedure Name: getIterationResponse
## Precondition: iterationResponse window has been made and a response
##               made by user
## Postcondition: proper function call is made in generator to send
##                response of user to engine
## Description: the proper function call is made to the generator
##
#####################################################################

proc ::inputValidator::getIterationResponse {} {
    ::generator::generateMesg "ITERATIONRESPONSE"
}


####################################################################
## Procedure Name: getInfoResponse
## Precondition: none
## Postcondition: none
## Description: not yet implemented due to lack of needed functionality
##
#####################################################################

proc ::inputValidator::getInfoResponse {} {
    
}


####################################################################
## Procedure Name: getErrorResponse
## Precondition: none
## Postcondition: none
## Description: not yet implemented due to lack of needed functionality
##
#####################################################################

proc ::inputValidator::getErrorResponse {} {
                
}


####################################################################
## Procedure Name: getName
## Precondition: the login window is activated
## Postcondition: the entry in the user name box has been stored in the
##                uiValues::userName variable
## Description: the procedure
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
