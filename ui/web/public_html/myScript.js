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
    document.writeln("<p><A href=\"javascript:void(0)\" onClick=\"list()\">List models<a>");
    document.writeln("<p><A href=\"create.html\">Create models<a>");
    document.writeln("<p><A href=\"javascript:void(0)\" onClick=\"available()\">List available actions<a>");
    document.writeln("<p><A href=\"run.html\">Run processes<a>");
    document.writeln("<p><A href=\"javascript:void(0)\" onClick=\"running()\">List running processes<a>");
    document.writeln("<p><A href=\"done.html\">Terminate processes<a>");


    document.writeln("<p><A href=\"logout.html\">Log out</a>");
    document.writeln("<br>");
    document.writeln("<img src=\"invisibleBar.gif\" width=100 height=1 alt=\"\">");
    writelnHiddenForm();
}


