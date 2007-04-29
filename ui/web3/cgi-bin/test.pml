process test {
iteration i1 {
action act_1 {
requires {  req_res.value == "True"  }
provides {  prov_res.value == "False"  }
script { "
	    This is the script, on a <i>separate</i> line.
	  " }
}
}
branch br1 {
action act_2 {
requires {  req_res.value == "False"  }
provides {  prov_res.value == "True"  }
script { "This is the script, on the <i>same</i> line, full stop." }
}
}
selection sel1 {
action act_3 {
requires {  req_res.value == "True"  }
provides {  prov_res.value == "False"  }
script { "
	    This is the script, on a separate line.
	  " }
}
}
sequence seq1 {
action act_4 {
requires {  req_res.value == "Passed"  }
provides {  prov_res.value == "Failed"  }
script { "
	    This is the script, on a separate line.
	  " }
}
}
}
