namespace eval ::uiLogin {

}

proc ::uiLogin::uiLoginScreen {} {


toplevel .uiloginwin -class "Login Screen"

wm title .uiloginwin  "PEOS - UI Module       Login Screen"
wm geometry .uiloginwin  325x150+40+40

frame .uiloginwin.usrid
pack .uiloginwin.usrid

frame .uiloginwin.pwd
pack .uiloginwin.pwd

pack .uiloginwin.pwd -after .uiloginwin.usrid

frame .uiloginwin.but -bd 2 -relief groove
pack .uiloginwin.but -after .uiloginwin.pwd -side bottom

label .uiloginwin.usrid.userid  -text "UserId: "
entry .uiloginwin.usrid.username -width 15 -bg WHITE
pack .uiloginwin.usrid.userid -side left -in .uiloginwin.usrid
pack .uiloginwin.usrid.username -side left -in .uiloginwin.usrid

label .uiloginwin.pwd.password -text "Password: "
entry .uiloginwin.pwd.access -width 20 -bg WHITE
pack .uiloginwin.pwd.password -side left -in .uiloginwin.pwd
pack .uiloginwin.pwd.access  -side left  -in .uiloginwin.pwd

.uiloginwin.pwd.access configure -show *

button .uiloginwin.but.login -text LogIn \
        -command {::uiProcess::uiProcessScreen}

button .uiloginwin.but.logout -text LogOut \
        -command { exit }

button .uiloginwin.but.help -text Help \
        -command {::uiHelp::uiLoginHelp}


pack .uiloginwin.but.login .uiloginwin.but.logout -ipadx 2 -pady 1 -ipady 1 -side left -in .uiloginwin.but
pack .uiloginwin.but.help -side right -in .uiloginwin.but
pack configure .uiloginwin.but.help -ipadx 2 -pady 1 -ipady 1
pack configure .uiloginwin.usrid .uiloginwin.pwd -padx 70  -pady 10
pack configure .uiloginwin.usrid .uiloginwin.pwd -anchor e

#tkwait window .uiloginwin

}
