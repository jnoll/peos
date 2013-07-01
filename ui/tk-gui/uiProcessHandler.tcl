namespace eval ::uiProcessHandler {

}

proc ::uiProcessHandler::uiProcessScreen {} {

destroy .uiloginwin

toplevel .uiprocesswin -class "Process Screen"

wm title .uiprocesswin "PEOS - UI Module           Process Screen"
wm geometry .uiprocesswin 500x400+40+40

frame .uiprocesswin.info -bd 1 -relief raised
pack .uiprocesswin.info -ipadx 50 -ipady 10 -padx 20 -pady 20

frame .uiprocesswin.procframe
pack .uiprocesswin.procframe

pack .uiprocesswin.procframe -after .uiprocesswin.info

frame .uiprocesswin.listframe
pack  .uiprocesswin.listframe

pack .uiprocesswin.listframe -after .uiprocesswin.procframe

frame .uiprocesswin.procbut -bd 2 -relief groove 
pack .uiprocesswin.procbut  -after .uiprocesswin.listframe

label .uiprocesswin.info.usrnm -text "User Name:"
entry .uiprocesswin.info.nm -text PEOS
pack .uiprocesswin.info.usrnm .uiprocesswin.info.nm -side left -in .uiprocesswin.info


label .uiprocesswin.procframe.processlst -text "List of Processes"
pack .uiprocesswin.procframe.processlst -ipadx 50 -ipady 20 -side top -in .uiprocesswin.procframe

listbox .uiprocesswin.listframe.listprocess -yscrollcommand {.uiprocesswin.listframe.sbar set }
.uiprocesswin.listframe.listprocess insert 0 Process1 Process2 Process3 Process4 \
Process5 Process6 Process7 Process8 Process9 Process10 Process11 Process12 

scrollbar .uiprocesswin.listframe.sbar -command { .uiprocesswin.listframe.listprocess yview}
pack .uiprocesswin.listframe.listprocess -ipadx 1 -ipady 1 -padx 6 -side left -anchor n \
-in .uiprocesswin.listframe 
pack .uiprocesswin.listframe.sbar -side left  -anchor n -fill y \
-in .uiprocesswin.listframe


button .uiprocesswin.procbut.action -text Action \
       -command { ::uiActionHandler::uiActionScreen }
 
button .uiprocesswin.procbut.back -text Back \
       -command { ::uiInputValidator::uiLoginScreen }

button .uiprocesswin.procbut.logout -text Logout \
       -command { exit }

pack .uiprocesswin.procbut.action .uiprocesswin.procbut.back .uiprocesswin.procbut.logout -side left \
-in .uiprocesswin.procbut

}
