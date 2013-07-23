proc isTrue { v } {
  return [expr {![string is false ${v}]}]
}

proc default { path } {
    exists $path
}

proc exists { path } {
    if {[catch {set r $path}]} {
        return 0
    }
    expr [file exists $path]
}

proc ax { path } {
    return 5
}

proc filecount { path } {
    if {[catch {set r $path}]} {
        return 0
    }
    if {![file exists $path]} {
        return 0
    }
    set i 0
    foreach f [exec ls $path] {
        incr {i}
    }
    expr $i
}

proc filesize { path } {
    if {[catch {set r $path}]} {
        return 0
    }
    if {![file exists $path]} {
        return 0
    }
    expr [file size $path]
}

proc timestamp { path } {
    if {[catch {set r $path}]} {
        return 0
    }
    if {![file exists $path]} {
        return 0
    }
    expr [file mtime $path]
}

proc misspellcount { path } {
    if {[catch {set r $path}]} {
        return 0
    }
    if {![file exists $path]} {
        return 0
    }
    set i 0
    foreach f [exec spell $path] {
        incr {i}
    }
    expr $i
}

source "hl7_parser.tcl"

proc glucose_test { path } {
	# checks that glucose is present in the OBX of the hl7 file
	set obxIds [getObxIdentifiers [getMsgFromFile $path]]
	foreach obxId $obxIds {
		set obxId [string tolower $obxId]
		if  {[string first "glucose" $obxId] >= 0} {
			return true
		}	
	}
	return false
}

 proc cholesterol_test { path } {
	# checks that ldl, cholesterol, hdl and triglycerides are present in the OBXs of the hl7 file
	set obxIds [getObxIdentifiers [getMsgFromFile $path]]
	set ldl false
	set cholesterol false
	set hdl false
	set triglycerides false
	foreach obxId $obxIds {
		set obxId [string tolower $obxId]
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
	return $ldl && $cholesterol && $hdl && $triglycerides
}
