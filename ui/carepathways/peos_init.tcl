if {[file exists "kernel_init.tcl"]} {
  source "kernel_init.tcl"
}

if {[file exists "hl7_parser.tcl"]} {
  source "hl7_parser.tcl"
}

proc get_patient_record { patientID } {
	return "${patientID}.hl7"
}


proc glucose_test { resource } {
	set logfile [open "log.txt" a+]
	puts $logfile "glucose_test 3"
	puts $logfile $resource
	puts $logfile "glucose_test after resource"
	# checks that glucose is present in the OBX of the hl7 file
	set obxIds [getObxIdentifiers [getMsgFromFile $resource]]
	puts $logfile "glucose_test after set obxIds"
	foreach obxId $obxIds {
		puts $logfile $obxId
		set obxId [string tolower $obxId]
		puts $logfile $obxId
		if  {[string first "glucose" $obxId] >= 0} {
			puts $logfile true
			return true
		}	
	}
	puts $logfile "glucose_test end"
	return false
}

 proc cholesterol_test { resource } {
	#return 1
	set logfile [open "log.txt" a+]
	puts $logfile "cholesterol_test 2"
	# checks that ldl, cholesterol, hdl and triglycerides are present in the OBXs of the hl7 file
	puts $logfile $resource
	puts $logfile "cholesterol_test after resource"
	set obxIds [getObxIdentifiers [getMsgFromFile $resource]]
	puts $logfile "cholesterol_test after set obxIds"
	set ldl false
	set cholesterol false
	set hdl false
	set triglycerides false
	puts $logfile "cholesterol_test before foreach"
	foreach obxId $obxIds {
		puts $logfile "cholesterol_test inside foreach"
		set obxId [string tolower $obxId]
		puts $logfile obxId
		if  {[string first "ldl" $obxId] >= 0} {
			set ldl true
		}	
		if  {[string first "cholesterol" $obxId] >= 0} {
			set cholesterol true
		}
		if  {[string first "hdl" $obxId] >= 0} {
			set hdl true
		}
		if  {[string first "triglycerides" $obxId] >= 0} {
			set triglycerides true
		}
	}
	puts $logfile "cholesterol_test before if"
	return [expr $ldl && $cholesterol && $hdl && $triglycerides]
}


proc suspect_diabetes { resource } {
	set logfile [open "log.txt" a+]
	puts $logfile "suspect_diabetes"
	puts $logfile $resource
	set diagnoses [getDiagnoses [getMsgFromFile $resource]]
	foreach diagnosis $diagnoses {
		set diagnosis [string tolower $diagnosis]
		if {$diagnosis == "suspect diabetes"} {
			return true
		}
	}
	return false
}

proc diagnosis {resource} {
	set diagnoses [getDiagnoses [getMsgFromFile $resource]]
	if {$diagnoses=={}} {
		return false
	} else {
		return true
	}
}

proc diabetes {resource} {
  set logfile [open "log.txt" a+]
	puts $logfile "diabetes"
	puts $logfile $resource
	set diagnoses [getDiagnoses [getMsgFromFile $resource]]
	foreach diagnosis $diagnoses {
		set diagnosis [string tolower $diagnosis]
		if {$diagnosis == "diabetes"} {
			return true
		}
	}
	return false
}


