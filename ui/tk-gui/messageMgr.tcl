#######################################################################
## File Information: $Id: messageMgr.tcl,v 1.1 1999/05/06 04:24:54 jneuder Exp $
#######################################################################

# messageMgr.tcl

package provide messageMgr 1.0

namespace eval ::messageMgr {

}


####################################################################
## Procedure Name: processList
## Precondition: parser calls for a process selection screen
## Postcondition: process selection screen has been created
## Description: calls makeProcessSelect procedure
## Arguments: none
##
#####################################################################

proc ::messageMgr::configUI { messageType } {

    switch $messageType {

      processList {
        ::dynamicWidgets::makeProcessSelect
      }

      choiceList  {
        ::dynamicWidgets::makeChoiceList  
      }

      action {
        ::dynamicWidgets::makeAction  
      }
         
      iterationComplete {
        ::dynamicWidgets::makeIterationComplete
      }  
        
      info {
        ::dynamicWidgets::makeInfo
      }

      error {
        ::dynamicWidgets::makeError
      }

      default  {
          ::uiErrorHandler::SendMsg \
              [format "%s Unrecognized message type %s" \
                   $ModuleName $mesg_type ]
      }
   }
}

