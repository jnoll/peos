#######################################################################
## File Information: $Id: messageMgr.tcl,v 1.2 1999/05/06 20:55:37 jneuder Exp $
#######################################################################

# messageManager script
# original code by Erik Koskinen, April 1999
# UI group from UCD CSC 5728 (Sofware Engin.) Spring 1999, Dr. Noll
# messageMgr.tcl

package provide messageMgr 1.0

namespace eval ::messageMgr {

}


####################################################################
## Procedure Name: configUI
## Precondition: parser makes a call for a screen
## Postcondition: call for screen creation has been made
## Description: a switch statement with message types decides which
##		screen to create
## Arguments: type of message received by parser
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

