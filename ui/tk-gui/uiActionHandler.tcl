namespace eval ::uiActionHandler {

}

proc uiActionHandler::uiActionScreen {} {

destroy .uiprocesswin

toplevel .uiactionwin -class "Action Screen"

frame .uiactionwin.ainfo -bd 1 -relief raised
pack .uiactionwin.ainfo -ipadx 120 -ipady 5 -padx 20 -pady 15

frame .uiactionwin.aframe
pack .uiactionwin.aframe

pack .uiactionwin.aframe -after .uiactionwin.ainfo

frame .uiactionwin.alistframe
pack .uiactionwin.alistframe

pack .uiactionwin.alistframe -after .uiactionwin.aframe

frame .uiactionwin.abut -bd 2 -relief groove
pack .uiactionwin.abut

pack .uiactionwin.abut -after .uiactionwin.alistframe


wm title .uiactionwin  "PEOS - UI Module                   Action Screen"
wm geometry .uiactionwin 500x400+40+40


label .uiactionwin.ainfo.usrnm -text "User Name:"
entry .uiactionwin.ainfo.nm -text PEOS
label .uiactionwin.ainfo.pnm -text "Process Name:"
entry .uiactionwin.ainfo.pnam -text PEOS
pack .uiactionwin.ainfo.usrnm .uiactionwin.ainfo.nm .uiactionwin.ainfo.pnm .uiactionwin.ainfo.pnam \
-side left -in .uiactionwin.ainfo 

label .uiactionwin.aframe.actionlst -text "List of Actions For Process"
pack .uiactionwin.aframe.actionlst -side top

listbox .uiactionwin.alistframe.listaction -yscrollcommand {.uiactionwin.alistframe.sbar set }
.uiactionwin.alistframe.listaction insert 0 Action1 Action2 Action3 Action4 \
Action5 Action6 Action7 Action8 Action9 Action10 \
Action11 Action12 Action13 Action14 Action15


scrollbar .uiactionwin.alistframe.sbar -command { .uiactionwin.alistframe.listaction yview}

entry .uiactionwin.alistframe.estatus -text "READY"
label .uiactionwin.alistframe.astatus -text "Status: "
pack .uiactionwin.alistframe.estatus  .uiactionwin.alistframe.astatus -side right  -anchor n \
-in .uiactionwin.alistframe
#pack .uiactionwin.alistframe.estatus -anchor n \
#-in .uiactionwin.alistframe

pack .uiactionwin.alistframe.listaction -ipadx 1 -ipady 1 -padx 6 -side left -anchor n \
-in .uiactionwin.alistframe 

pack .uiactionwin.alistframe.sbar -side left -anchor n -fill y \
-in .uiactionwin.alistframe 

button .uiactionwin.abut.execute -text Execute

button .uiactionwin.abut.info -text Info \
       -command {uiActionHandler::uiActionInfo}
       

button .uiactionwin.abut.back -text Back \
       -command {::uiProcessHandler::uiProcessScreen}

button .uiactionwin.abut.logout -text Logout \
       -command { exit }

pack .uiactionwin.abut.execute .uiactionwin.abut.info .uiactionwin.abut.back .uiactionwin.abut.logout -side left 

}

proc uiActionHandler::uiActionInfo {} {

set .uiInfowin .infotext
catch {destroy .uiInfowin}

toplevel .uiInfowin -class "Action Information"
wm title .uiInfowin "PEOS - UI Module                               Action Information Screen"
wm geometry .uiInfowin 500x400+40+40

frame .uiInfowin.infobutframe
pack .uiInfowin.infobutframe -side bottom -fill x -pady 2m


text .uiInfowin.infotext -relief sunken -bd 3 -yscrollcommand ".uiInfowin.scroll set" -setgrid 1 \
-height 40
scrollbar .uiInfowin.scroll -command ".uiInfowin.infotext yview"
pack .uiInfowin.scroll -side right -fill y
pack .uiInfowin.infotext -expand yes
.uiInfowin.infotext insert 0.0 \
ACTIONINFORMATION WILL BE DISPLAYED HERE.


button .uiInfowin.infobutframe.infoback -text Return -command "destroy .uiInfowin"
pack .uiInfowin.infobutframe.infoback

}

#uiActionHandler::uiActionScreen
