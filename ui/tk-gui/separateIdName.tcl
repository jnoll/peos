#######################################################################
## File Information: $Id: separateIdName.tcl,v 1.1 1999/05/06 04:24:54 jneuder Exp $
#######################################################################

#######################################################################
# Function name:  separateIdName
# Precondition:   Separates the name from its identifier and stores
#                 both in a list.
# Postcondition:  The text for the specified name field is extracted
# Description:    The name and its id are stored in a list
# Arguments:
#                 *  name_id_field   -  The name field to read from
#######################################################################

package provide parser 1.0

namespace eval ::parser  {
   namespace export separateIdName
}

proc ::parser::separateIdName { name_id_field }  {
   
   # Get the field length
   set name_id_field_len [string length $name_id_field]

   # Find the separator position
   set id_sep_index [string first "/" $name_id_field]

   # If there is no "/" then we only have a name
   if { $id_sep_index == -1 }  {

      # If field completely empty return empty string 
      if { $name_id_field_len == 0 } {
           return "" 
      }

        lappend sublist [string range $name_id_field 0 [expr $name_id_field_len -1]]
      return $sublist
   }
  
   # Get name into the list
   lappend sublist [string range $name_id_field 0 [expr $id_sep_index - 1]]

   # Get the associated identifier into the list
   lappend sublist [string range $name_id_field \
                                 [expr $id_sep_index + 1] [expr $name_id_field_len -1]]

   return $sublist
}
