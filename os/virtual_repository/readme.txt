PEOS Virtual_Repository Release Version 1.0
READ ME

Dear PEOS Customer,

Welcome and thank you for choosing the PEOS software for your process 
management activities. This file contains information you need for 
use the software. 

Please direct all technical assistance request to task4one@yahoo.com

Running PEOS Virtual_Repository:
	
	1) 	Change directory (cd) to virtual_repository.
	
	2)	Type "make" at the command prompt and ENTER to initialize 
		the system.
	
	3) 	Enter a query as "ATTRIBUTE OPERATOR VALUE".
		
		NOTE: 	Look at the list of valid attributes and operators below.
		
Exiting PEOS Virtual_Repository:

	1)	Press "CTRL-C" simultaneously.		
	 
Valid Attributes:

	1) 	ID (Unique Identifier).
	
Valid Operators:

	1) 	EQ (Equal).
	
Valid VALUE:

	1) 	Unix file system url (file://filename)
	
		NOTE: 	If value is file://filename, PEOS searches from your current working directory for the file.

	 
	2) 	Unix file system url (file:///fullpath/filename)
	
		NOTE: 	If value is file:///fullpath/filename, PEOS returns the existence of the file. 

Sample run:

	Directory Structure
	-------------------
	
	[ jnoll@linux103 : /users/faculty/jnoll ] > ls -l

	-rw-------    1 jnoll     faculty             0 Mar 13 16:47 a.c
	drwx------    9 jnoll     faculty          4096 Mar  5 19:11 peos/
	
	[ jnoll@linux103 : ~/peos/src/os/virtual_repository ] > ls -l
	
	drwxrwxrwx   2 jnoll       faculty       4096 Mar 13 17:37 CVS
	-rwxrwxrwx   1 jnoll       faculty        551 Feb 26 17:05 Mailseeker.c
	drwxrwxrwx   5 jnoll       faculty       4096 Mar 13 17:36 testSuites
	-rw-r--r--   1 jnoll       faculty       5705 Mar 13 17:34 vrepo.c

	[ jnoll@linux103 : /users/faculty/jnoll/peos/src/os/virtual_repository/testSuites ] > ls -l

	drwxrwxrwx    2 jnoll     faculty          4096 Mar 13 17:37 CVS/
	drwxrwxrwx    3 jnoll     faculty          4096 Mar  7 18:32 testDir/
	-rwxrwxrwx    1 jnoll     faculty          3259 Mar 13 17:36 testFSsearchSeek.c
	
	[ jnoll@linux103 : /users/faculty/jnoll/peos/src/os/virtual_repository/testSuites/testDir ] > ls -l

	-rwxrwxrwx    1 jnoll     faculty             0 Mar  7 18:32 testfile1.c

		
	1)	Input				
		
		ID EQ file://vrepo.c		
	
		Output
	
		1 record(s) found!
		file://vrepo.c


	2)	Input
	
		ID EQ file:///home/jnoll/a.c
		
		Output
		
		1 record(s) found!
		file:///home/jnoll/a.c


	3)	Input
	
		ID EQ file:///home/jnoll/peos/src/os/virtual_repository/Mailseeker.c

		Output
		
		1 record(s) found!
		file:///home/jnoll/peos/src/os/virtual_repository/Mailseeker.c
		

	4)	Input
	
		ID EQ file:///a.c
		
		Output
		
		queries seeks file:///a.c
		
		
	5)	Input
	
		ID EQ file://testSuites/testDir/testfile1.c
		
		Output
		
		1 record(s) found!
		file://testSuites/testDir/testfile1.c
		

	6)	Input
	
		ID EQ file:///home/jnoll/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c
		
		Output
	
		1 record(s) found!
		file:///home/jnoll/peos/src/os/virtual_repository/testSuites/testDir/testfile1.c	
