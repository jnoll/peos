if {[file exists "kernel_init.tcl"]} {
  source "kernel_init.tcl"
}

proc diagnosis {resource} {
  return true
}

proc glucose {resource} {
  return true
}

proc suspect_diabetes {resource} {
  return true
}

proc cholesterol {resource} {
  return true
}
