Java GUI For PEOS README.txt

This file is intended to help a new user through the setup of the Java-GUI
Peos application. All intended requirements for the program are listed below,
followed by a general overview of the process model.

It is our hope that the application is straightforward to use and provides you
with bug-free service. Please feel free to report any bugs or errors to the
email address listed below.


-[ Requirements ] ----------------------------------------------------

- Linux Workstation
- Xerces 2.6.2 

-[ Installation ] -----------------------------------------------------------

First and foremost, you need to set the following lines in the Makefile.

--Lines required at compile time--
XERCES_PATH
 This is the path to where the Xerces class files are located.
JUNIT_PATH
 This is the path to where the Junit class files are located. 

--Lines required to create runpeos script (make script)--
path
 This is the path to the peos binary. Please include the binary in this path
 ex: /home/username/peos/bin/peos
images 
 This is the path for all state and menu icons in the program.
help
 This is the path for all help HTML files.
pml
 This is the default path for loading PML files.
JAVAGUI_JAR
 This is the path to the compiled java gui jar file created by running "make". 
 After running "make", edit this line for make script to run properly.

The defaults assume you are going to load, compile, and run the program from
the CVS module directory. If this is the case, the lines do not need to be
modified. For a server-wide install though, these paths will need to be set
and the following line will need to be run:

% make script

After you have run the script, assuming the appropriate pathnames have been
entered, you can run the Java GUI for PEOS.

% ./runpeos

-[ Verify ] ----------------------------------------------------------
Make sure that all path names are set properly and that copies of the .pml
files included in the CVS module are stored in your pml directory.

Path names that are relevent to unit tests:
 XERCES_PATH, path, JUNIT_PATH
 These _must_ be set for the unit tests to compile and run. 

Unit tests must be compiled and run from the base directory for the module.

Running Unit Test:
% make -s jtest 


-[ Running ] ---------------------------------------------------------
After following the installation instructions above, a script will be created
and all you have to do is type: 

% runpeos

1) Let's get started!
   A: After typing runPeos.sh, the UI for PEOS will appear onscreen.
      Load a file by selecting File->Load . A window will pop up 
      asking you to choose a file. Choose the .pml file of choice 
      and it should pop up on the screen.

2) I have some processes open, what can I do now?
   A: When you open a process via File->Load or File->Active, a 
      window will pop up with an outline of available actions.
      Click on an action(the first action is viewed by default).
      This will display relevant information for that action in 
      the right pane. 

3) What do the buttons at the bottom of the screen mean?

   START:    As long as a process is not in the RUN state, you can
             start the process. This will move the file to the RUN
	     state. If resources are required, a window will pop
	     up, asking you to input them.

   FINISH:   As long as the process is not already DONE, you can 
	     finish the process. Like start, this will move the 
	     file to the DONE state. If required resources are
	     not inputted, a submit window will pop up.

   ABORT:    This button will change a process in the RUN state to 
	     the NONE state. 

   SUSPEND:  This button will change a process in the RUN state to
	     the SUSPEND state.

   PREVIOUS: As long as there is an action previous to this action
	     in the list, you can cycle to the previous action by 
	     clicking this button.

   NEXT:     As long as there is an action after this action in the list,
	     you can cycle to the next action by clicking this button.

4) How do I delete a process?
   A: Deleting a process is similar to viewing an active process.
   You must first select the process from the File->Delete menu,
   and then a confirmation box will appear. If you are sure you
   wish to delete the process, click OK to do so.

5) How do I quit PEOS?
   A: Quitting PEOS is simple. Either click the X in the upper right 
   corner of the PEOS window, or select File->Quit. 

6) What happens to my files when I quit PEOS?
   A: Your files are saved in the local directory of the PEOS application.
   Information for all active processes are stored here. If the data 
   somehow becomes corrupted, you must remove these files:
	% rm proc_table.*
        % rm event.log

7) Check the online help for more information! It's really good!

-[ Troubleshooting ] -------------------------------------------------

1)
  Q: When I try to load an application, nothing happens!
  A: Make sure all paths are set appropriately and re-run make script. 

  
 

-[ Contacting Us ] ---------------------------------------------------

Please direct any inquiries to:

	Steve Beeby
	Email: shinobi@club-vette.com
