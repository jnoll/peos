#######################################################################
## File Information: $Id: extractName.tcl,v 1.2 1999/05/07 18:53:09 jneuder Exp $
#######################################################################

package provide generator 1.0

namespace eval ::generator  {
   variable ModuleName "generator\:\:"
   namespace export extractName
   namespace export extractId
   namespace export extractNameId
}


##################################################################
# Function name:  extractNameId
# Precondition:	  None
# Postcondition:  A name & id string is constructed
# Description:	  Extracts the name & id field in the list and
#		  build a string of this format:
#		   "name/id"
# Parameters:
#		  * name_id  -	a list of 2 elements: name & id
##################################################################

proc ::generator::extractNameId { name_id }  {

    set name [lindex  $name_id	0]

    set listlen [llength $name_id]
    if {$listlen == 2}	{
	set id [lindex	$name_id  1]
	set result [format "%s/%s" $name $id]
    }  else  {
	set result [format "%s" $name]
    }

    return $result
}



##################################################################
# Function name:  extractName
# Precondition:	  None
# Postcondition:  The name field is extracted
# Description:	  Extracts the first field in the given list
# Parameters:
#		  * name_id  -	a list of 2 elements: name & id
##################################################################

proc ::generator::extractName { name_id }  {

    set name [lindex  $name_id	0]
    return $name
}


##################################################################
# Function name:  extractId
# Precondition:	  None
# Postcondition:  The id field is extracted
# Description:	  Extracts the second field in the given list
# Parameters:
#		  * name_id  -	a list of 2 elements: name & id
##################################################################

proc ::generator::extractId { name_id }	 {

    set listlen [llength $name_id]

    if {$listlen == 2}	{
	set id [lindex	$name_id  1]
    }  else  {
	set  id ""
    }

    return $id
}
