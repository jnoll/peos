#######################################################################
## File Information: $Id: readMesgField.tcl,v 1.1 1999/05/06 04:24:54 jneuder Exp $
#######################################################################

#######################################################################
# Function name:  readMesgField
# Precondition:   A message is read from UI and its type is parsed
#                 by parseMesg
# Postcondition:  The text for the specified mesg field is extracted
# Description:    Reads the text of the specified field in the
#                 given message.
# Arguments:
#                 *  mesg     -  The message to read from
#                 *  keyword  -  The field whose text we want to read
#######################################################################

package provide parser 1.0

namespace eval ::parser  {
   namespace export readMesgField
}

proc ::parser::readMesgField { mesg keyword }  {

   # Capitalize the keyword
   set keyword [string toupper $keyword]

   # Add ":" to the last position of the keyword to differentiate them
   set keyword [format "%s:" $keyword]

   # Add "|" to the last position of the message
   set mesg [format "%s|" $mesg]

   # Make copy of a capitalized version of the message to search for keyword
   set cap_mesg [string toupper $mesg]

   # Get the original message length
   set orig_len  [string length $mesg]

   # Find the index of the 1st appearance of the keword 
   # in the message
   set index [string first $keyword $cap_mesg]
   # Keyword not found
   if { $index == -1 }  {
      return "" 
   }

   # Get the substring starting from the found keyword
   # to the rest of the message
   set submesg [string range $mesg $index [expr $orig_len - 1]]
   set sub_len [string length $submesg]
   
   # Search for the semicolon that separates the keyword
   # from the field text
   set sub_index [string first ":" $submesg]

   # The field text starts from here until the "|" char
   set submesg [string range $submesg $sub_index $sub_len]
   set end_index [string first "|" $submesg]
   set submesg [string range $submesg 1 $end_index]

   # Look for list separator "::".  If none exists, this is a 
   # single text field, return the text string in that case.
   while { [string length $submesg] != 0 }  {
      set end_index [string first "|" $submesg]
      set sep_index [string first "," $submesg]
      # The last text field
      if { $sep_index == -1 }  {
           set name_field [string range $submesg 0 [expr $end_index - 1]]
           set sub_list [separateIdName $name_field]
           lappend result $sub_list
           #lappend result [string range $submesg 0 [expr $end_index - 1]]
           set submesg ""
      
      } else {
             set name_field [string range $submesg 0 [expr $sep_index - 1]]
             set sub_list [separateIdName $name_field]
           #set listitem [string range $submesg 0 [expr $sep_index - 1]]
           lappend result $sub_list
           # Cut the read item from the message
           set strlen [string length $submesg]
           set submesg [string range $submesg [expr $sep_index + 1] \
                                              [expr $strlen - 1]]
      }
   }
    
   return $result
}
