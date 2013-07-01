namespace eval ::uiHelp {

}

proc ::uiHelp::uiLoginHelp {} {
tk_messageBox -icon info -type ok -title "PEOS - UI Module Login Help" \
-message "Type Correct UserName & PassWord" -parent .
}

proc ::uiHelp::uiProcessHelp {} {
tk_messageBox -icon info -type ok -title "PEOS-UI Module                    Process Screen Help" \
-message "Actions Are Displayed in a List Box For The User Logged In.\n  
Select The Process and Press the Action Button To View The Actions Available for the Process And To Execute Them \n
To go back to the Login Screen Press The Back Button \n
To Exit PEOS press Logout Button"
}

proc ::uiHelp::uiActionHelp {} {

tk_messageBox -icon info -type ok -title "PEOS-UI Module                    Action Help Screen" \
-message "Actions For The Selected Process are shown on this screen. \n
To Execute an Action Press The Execute Button. Only Actions in Ready State Can Be Executed. \n
To View Information About An Action Select The Action and Press Info. Button. \n
To go back To The Process Screen Press The Back Button. \n
To Exit PEOS press Logout Button."
}


#::uiHelp::uiActionHelp


