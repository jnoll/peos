process start_test {
	  action test_script {
	    requires{"test_resource"}
  	    script{"this is a script"}
	  }
}
