function writeHiddenForm(){
    document.writeln("<form method=\"post\" action = \"webui.cgi\" name = \"webuiForm\">");
    document.writeln("<INPUT TYPE = \"hidden\" NAME = \"command\">");
    document.writeln("<INPUT TYPE = \"hidden\" NAME = \"action\">");
    document.writeln("<input type = \"hidden\" name = \"sessionid\">");
    document.writeln("<input type = \"hidden\" name = \"username\">");
    document.writeln("<input type = \"hidden\" name = \"password\">");
    document.writeln("</form>");
}

function writeMenu(userType){
    document.writeln("<TR><TD WIDTH=\"100\" VALIGN=\"TOP\" ALIGH=\"CENTER\" BGCOLOR=\"94D6E7\" HEIGHT=\"100%\">");

    document.writeln("<p><A href=\"main.html\">home</a>");

    /*
    if (userType==2) {
        document.writeln("<p><A href=\"stub.html\">create a new user </a>");
        document.writeln("<p><A href=\"stub.html\">delete a user </a> ");
        document.writeln("<p><A href=\"stub.html\">reset a password </a>");
    }
    if (userType==2 || userType==1) {
        document.writeln("<p><A href=\"stub.html\">create a new process </a>");
        document.writeln("<p><A href=\"stub.html\">terminate a process </a>");
        document.writeln("<p><A href=\"stub.html\">actions </a>");
    }
    if (userType==3 || userType==1) {
        document.writeln("<p><A href=\"stub.html\">monitor a process </a>");
        document.writeln("<p><A href=\"stub.html\">actions </a>");
    }
    */

    //for commands requiring parameters we take them to a different page
    //for commands NOT requiring parameters we invoke a cgi program that can generate the page
    document.writeln("<p><A href=\"javascript:invokeAction('create', '')\">List models<a>");
    document.writeln("<p><A href=\"create.html\">Create models<a>");
    document.writeln("<p><p><A href=\"javascript:invokeAction('status', '')\">List available actions<a>");
    document.writeln("<p><A href=\"run.html\">Run processes<a>");
    document.writeln("<p><p><A href=\"javascript:invokeAction('running', '')\">List running processes<a>");
    document.writeln("<p><A href=\"done.html\">Terminate processes<a>");

    document.writeln("<p><A href=\"logout.html\">Log out</a>");

    if(userType==1){
        document.writeln("<p><A href=\"stub.html\">user type 1 actions</a>");
    }
    else if(userType==1){
        document.writeln("<p><A href=\"stub.html\">user type 2 actions</a>");
    }
    else{
        document.writeln("<p><A href=\"stub.html\">user type 3 actions</a>");
    }

    document.writeln("<br>");
    document.writeln("<img src=\"invisibleBar.gif\" width=100 height=1 alt=\"\">");

    writeHiddenForm();
    document.writeln("</TD>");

}

function writePeosTitle(){

    document.writeln("<TABLE CELLSPACING=\"0\"  CELLPADDING=\"10\" BORDER=\"0\" WIDTH=\"100%\" HEIGHT=\"100%\" ALIGH=\"CENTER\">");

    document.writeln("<TR><TD HEIGHT=\"75\" ALIGH=\"CENTER\" COLSPAN=2 BGCOLOR=\"94D6E7\">");
    document.writeln("<P><img src=\"peos_title.gif\" width=470 height=75 alt=\"\" border=0></TD>");
    document.writeln("</TR>");

}
