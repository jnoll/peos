PEOS Virtual_Repository version 1.1
READ ME

Dear PEOS Customer,

Welcome and thank you for choosing the PEOS software for your process 
management activities. This file contains information you need for 
use the software. 

Running PEOS Virtual_Repository:
	
	1) 	Change directory (cd) to virtual_repository.
	
	2)	Type "make" at the command prompt and ENTER initiate 
		the system.
	
	3) 	Enter a query as "ATTRIBUTE OPERATOR VALUE".
		
		NOTE: 	Look at the list of valid attributes and operators below.
		
		NOTE: 	By default, the search for unixfs begins at the installed directory.
			To change the search directory open config.txt and give the SEARCHDIR 
			the name of the desired starting directory.
		
Exiting PEOS Virtual_Repository:

	1)	Press "CTRL C" simultaneously.	
	
	 
Valid Attributes:

	1) ID (Identifier).
	
Valid Operators:

	1) EQ (Equal).
	
Valid VALUE:

	1) File system url (file://filename)
	NOTE: If path is just a filename then the search for the specified file.
	 
	2) File system url (file://path)
	NOTE: Here the path is the complete path for the file. The system will 
	try to check the existence of the file in the specified path.    
	
	



