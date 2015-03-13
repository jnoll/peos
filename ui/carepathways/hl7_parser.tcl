 proc processData { channel } {
    #Do we have a full message?
	#The socket/channel must be [fconfigure $channel -translation binary -blocking 0 -buffering none] in your accept connection [proc], otherwise it won't work.
    set begin [string first \xb $::peers($channel.data)]
    set end [string first \x1c $::peers($channel.data)]
    set sender ""
    set msgid ""
    if { $end > $begin } {
	set msg [string range $::peers($channel.data) [expr $begin +1] [expr $end-1]]
	foreach line [split $msg \xd] {
	    set fields [split $line |]
	    switch [lindex $fields 0] {
		MSH {
		    set sender [lindex $fields 2]
		    set msgid [lindex $fields 9]
		    set environ [lindex $fields 10]
		}
	    }
	}
	if { $sender ne "" && $msgid ne "" } {
            #TODO: insert check to see if the the message is a query response, those must not
            #be answered with ACK!
	    set ack "\xbMSH|^~\\&|ASTRAIA|astraia|$sender||||ACK||$environ|2.2||\xdMSA|AA|$msgid||\xd\x1c\xd"
	    puts -nonewline $channel $ack
	    set ::peers($channel.data) [string range $::peers($channel.data) [expr $end + 1] end]

            #For debugging, I write all messages to a timestamped file, so I can replay them:
	    puts $::peers($::peers($channel).tsd) [list msg [clock clicks -milliseconds] $msg]
	    puts $::peers($::peers($channel).tsd) [list out [clock clicks -milliseconds] $ack]

            #And call the message handler:
	    processMsg $msg

	} else {
	    log "No sender/msgid $sender/$msgid"
	}
    }
 }
 
 
 proc processMsg { msg } {
    global db

    #first split the message into individual segments.
    set segments [split $msg \xd]
    foreach segment $segments {
			#as it is unlikely that we don't need to split on fields, I split
			#every segment on level one (|).

		set fields [split $segment |]

			#and for easy access everything goes into an array keyed on segment ID
			#which is, of course, a list!
		lappend seg([lindex $fields 0]) $fields
		switch [lindex $fields 0] {
			MSH {
					#this will set some variables in my environment prefixed MSH_
					#more on this later.
			hl7_set_segment_variables $fields
			}
				default {

				}
		}
    }

    #Choose what to do based on the message type from the MSH header segment.
    switch $MSH_type {
	ADT {
	    foreach pid $seg(PID) {
		#only update:
		processPID $db $pid 1
	    }
	}
	SIU {
	    processPID $db [lindex $seg(PID) 0] 0
	    hl7_set_segment_variables [lindex $seg(AIS) 0]
	    if { [lsearch {FE50 GR01 GR02 GR03} $AIS_code] == -1 } {
		log "Niet geintereseerd in SIU $AIS_code '$AIS_description'"
		return
	    }
	    hl7_set_segment_variables [lindex $seg(SCH) 0]
	    hl7_set_segment_variables [lindex $seg(AIL) 0]
	    hl7_set_segment_variables [lindex $seg(PID) 0]
	    switch $MSH_eventtype {
		S12 {
                    #worker code to insert appointment details into database goes here.
		}
		S15 {
                    #worker code to delete/cancel an appointment.
		    if { [string trim $SCH_id] ne "" } {
			ns_db dml $db "delete from diary where appointmentID=[ns_dbquotevalue $SCH_id] and application=[ns_dbquotevalue $SCH_application]"
                    }
		}
	    }
	}
    }
 }
 
 proc hl7_set_segment_variables { segment } {
    #segment contains a list of fields from a single segment (line) from the message.
    #this means, it has already been [split $line |]

    #the first element contains the segment ID:
    set seg [lindex $segment 0]

    #fields is a list with list indices (starting at one, because $seg is at 0!) and
    #desired field names. If a field is to be devided in subfields, the field name is
    #a list of field names and the field will be split again, this time with the level 2
    #character (usually the carret: ^)
    #because the messages I need to parse go no deeper than two levels, this code doesn't
    #go any further, but the same can of course be done for the next levels.

    #Note that my segments dictionaries are not complete!
    switch $seg {
	MSH {
	    set fields {
		1 encodingchars
		2 sendingapplication
		3 sendingfacility
		4 receivingapplication
		5 receivingfacility
		6 datetime
		8 {type eventtype}
		9 msgid
		10 processingid
		11 version
		12 sequencenumber
		13 continuationptr
	    }
	}
	SCH {
	    set fields {
		1 {id application}
		16 {fillercode fillername}
		20 {enteredbycode enteredbyname}
		25 statuscode
	    }
	}
	AIL {
	    set fields {
		1 setid
		2 actioncode
		3 {resourceid roomnumber}
		5 locationgroup
	    }
	}
	AIS {
	    set fields {
		1 setid
		3 {code description codingsystem}
		4 startdatetime
		7 duration
		8 durationunits
		9 allowsubstitution
		10 fillerstatus
	    }
	}
	PID {
	    set fields {
		1 setid
		2 {externalid externalid_checkdigit}
		3 {pid pid_checkdigit}
		4 alternateid
		5 {eigennaam roepnaam initialen achtervoegsels voorvoegsels titulatuur}
		7 birthdate
		8 sex
		11 {straatnaam huisnr woonplaats provincie postcode land}
		13 phonehome
		14 phonebus
		15 languagepatient
		16 maritalstatus
		17 religion
		23 birthplace
		26 citizenship
	    }
	OBX {
	    set fields {
		1 setId
		2 dataType
		3 observationIdentifier
		4 observationValue
		5 units
		6 referencesRange
		7 abnormalFlags
		8 probability
		9	natureOfAbnormalTest
		10 resultStatus
		11 dateOfLastNormalValues
		12 userDefinedAccessCheck
		13 dateTimeOfObservation
		14 producerId
		15 responsibleObserver
		16 observationMethod
	    }
	}	
	}
	default {
            #error or return for unknow segment types?
	    return
	}
    }

    #Now set the variables in the callers environment:
    foreach {fieldno names} $fields {
	if {[llength $names] == 1} {
            #A single variable name, no further split needed.

            #Use [upvar] to create/set a variable in the callers environment
	    upvar ${seg}_$names var

            #hl7string does nothing more than return an empty string when the field has
            #the 'empty' marker "" in it.
            #HL7 makes a difference between NULL values and empty string.
	    set var [hl7string [lindex $segment $fieldno]]
	} else {
            #More than one fieldname: split another level
            #Even better would have been the same syntax for each sublevel
            #and then call a function like [setsubsegment <segmentid> <fieldnames> <sepchars-for-remaining-levels>]

	    set subfields [split [lindex $segment $fieldno] ^]
	    set i 0
	    foreach name $names {
		upvar ${seg}_$name var
		set var [hl7string [lindex $subfields $i]]
		incr i
	    }
	}
    }
 }
 
 
 proc hl7string { s } {
    if { $s eq {""} } {
	return ""
    }
    return $s
 }

 proc hl7dateToISO { date } {
    set dpart [string range $date 0 7]
    set tpart [string range $date 8 13]
    set res ""

    if { [string length $dpart] == 8 } {
	append res "[string range $dpart 0 3]-[string range $dpart 4 5]-[string range $dpart 6 7] "
    }
    if { [string length $tpart] == 6 } {
	append res "[string range $tpart 0 1]:[string range $tpart 2 3]:[string range $tpart 4 5].000"
    } elseif { [string length $tpart] == 4 } {
	append res "[string range $tpart 0 1]:[string range $tpart 2 3]:00.000"
    }

    #log "Date $date $dpart/$tpart $res"
    return $res
 }

 proc removezeros { s } {
    regexp {^0+(.+)$} $s -> s
    return $s
 }
 
 
 proc getMsgFromFile { filename } {
	if {![file exists $filename]} {
	  return ""
	}
	set hl7file [open $filename r]
	set msg  [read $hl7file]	
	close $hl7file
	return $msg
 }
 
 proc getObxIdentifiers { msg } {
	#first split the message into individual segments.
    #set segments [split $msg \xd]
	set segments [split $msg "\n"]
	set obxIds {}

    foreach segment $segments {
			#as it is unlikely that we don't need to split on fields, I split
			#every segment on level one (|).
		set fields [split $segment |]			
		
		switch [lindex $fields 0] {
			OBX {
				lappend obxIds [lindex $fields 3]

			}
			default {

			}
		}
    }
	return $obxIds
 
 }
 
  proc getDiagnoses { msg } {
	#first split the message into individual segments.
    #set segments [split $msg \xd]
	set segments [split $msg "\n"]
	set diagnoses {}

    foreach segment $segments {
			#as it is unlikely that we don't need to split on fields, I split
			#every segment on level one (|).
		set fields [split $segment |]			
		
		switch [lindex $fields 0] {
			DG1 {			
				#puts [lindex $fields 3]
				set diagnosisFields [split [lindex $fields 3] "^"]
				#puts [lindex $diagnosisFields 1]
				lappend diagnoses [lindex $diagnosisFields 1]

			}
			default {

			}
		}
    }
	return $diagnoses
 
 }
 
 
 
 
 #puts [getObxIdentifiers [getMsgFromFile "cholesterol_test.hl7"]]
 #puts [glucose_test "glucose_test.hl7"]
 #puts [glucose_test "cholesterol_test.hl7"]
 #puts [cholesterol_test "glucose_test.hl7"]
 #puts [cholesterol_test "cholesterol_test.hl7"]
 #puts [getDiagnoses [getMsgFromFile "patient_record.hl7"]]
 
 
 