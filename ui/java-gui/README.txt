Peos Iteration 4b
23-May-2004

INSTALLING PEOS ITERATION 4b

To install the software, carefully follow the instructions below.

-[ Requirements ] ----------------------------------------------------

- Linux Workstation
- Xerces 2.6.2 

-[ Installation ] -----------------------------------------------------------

First and foremost, you need to set the following lines in the Makefile.
images
path
help
pml
XERCES_PATH
JUNIT_PATH
JAVAGUI_JAR

The defaults assume you are going to load, compile, and run the program from
the CVS module directory. If this is the case, the lines do not need to be
modified. For a server-wide install though, these paths will need to be set
and the following line will need to be run:

% make script

After you have run the script, assuming the appropriate pathnames have been
entered, you can run the Java gui for PEOS.

% ./runpeos

-[ Verify ] ----------------------------------------------------------
Make sure that all path names are set properly and that copies of the .pml
files included in the CVS module are stored in your pml directory. 

Unit tests must be compiled and run in a directory with all the source .java
files. 

Unit Test:
% make -s jtest 

All tests should pass OK. No failures should be reported.


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
