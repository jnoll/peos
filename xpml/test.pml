process test {
iteration i1 {
action act_1 {
requires { req_res.value == "expression1" }
provides { prov_res.value == "expression2" }
script { "
    This is the script, on a <i>separate</i> line.
" }
}
}
branch br1 {
action act_2 {
requires { req_res.value == "expression1" }
provides { prov_res.value == "expression2" }
script { "<i>This is the script, on the same line.</i>" }
}
}
selection sel1 {
action act_3 {
requires { req_res.value == "expression1" }
provides { prov_res.value == "expression2" }
script { "
    This is the script, on a separate line.
" }
}
}
sequence seq1 {
action act_4 {
requires { req_res.value == "expression1" }
provides { prov_res.value == "expression2" }
script { "
    This is the script, on a separate line.
" }
}
}
}
