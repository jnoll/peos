process test_action {
action test_action {
requires { test_resource }
provides { test_provide }
script { "this is a script" }
}
action test_script {
requires { test_resource }
provides { test_provide }
script { "this is a script" }
}
}
