Peos Iteration 4b
23-May-2004

INSTALLING PEOS ITERATION 4b

To install the software, carefully follow the instructions below.

-[ Requirements ] ----------------------------------------------------

- Linux Workstation
- Xerces 2.6.2 

-[ Build ] -----------------------------------------------------------

% cd peos
% make
% make setup

make setup is required for the program to work. This sets the path of PEOS
which is necessary for many methods in our PEOS interface to run. If the 
program does not seem to be running correctly, it is likely you did not 
enter a proper path for PEOS. 

Example: 

% make -s setup
% Please enter pathname for PEOS.
% /home/jshah1/bin/peos

Note that it is required to enter the path including the executable.

* Occasionally, this program will spit out some garbage when it checks
  the availability of the peos executable. Please ignore.

-[ Verify ] ----------------------------------------------------------
(Make sure to run "make setup" before you attempt the unit tests.)
Unit Test:
% make -s test 


If the above option displays:
displayPO: Pass: 9 Fail: 0
LoadProcess Pass: 3 Fail: 0
Buttons pass: 7 fail: 0
ActionMap pass: 6 fail: 0

The program has been verified and passes all unit tests.


-[ Running ] ---------------------------------------------------------

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
  A: Make sure you typed "make setup" before running runPeos.sh! If
     you have done this, odds are you did not enter a proper path.
     Verify the PEOS path and use runPeos.sh again.

  
 

-[ Contacting Us ] ---------------------------------------------------

Please direct any inquiries to:

	Steve Beeby
	Email: shinobi@club-vette.com

	Gloria Chang
	Email: gwchang@scu.edu	


