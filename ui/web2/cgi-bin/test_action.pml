process start_test {
	  action test_script {
	    requires{"test_resource"}
	    provides{"test_provide"}
  	    script{"this is a script"}
	  }
}
