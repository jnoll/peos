process seq {
  task br {

    action z manual {
      requires {"R.html"}
      provides {"R.html"}
    }

    iteration it {

      action a manual {
	requires {"R.html"}
	provides {"R.html"}
      }

      action b manual {
	requires {"R.html"}
	provides {"R.html"}
      }

      action c manual {
	requires {"R.html"}
	provides {"R.html"}
      }

      action d manual {
	requires {"R.html"}
	provides {"R.html"}
      }
    }
  }
}

class File {
  string contents ;
}

class Node extends File {
  int date 12 ;
  string author "John Noll" ;
}