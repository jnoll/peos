function writeMenu(userType){
    document.write("<p><A href=\"main.html\">home</a>");

    if (userType==2) {
        document.write("<p><A href=\"stub.html\">create a new user </a>");
        document.write("<p><A href=\"stub.html\">delete a user </a> ");
        document.write("<p><A href=\"stub.html\">reset a password </a>");
    }
    if (userType==2 || userType==1) {
        document.write("<p><A href=\"stub.html\">create a new process </a>");
        document.write("<p><A href=\"stub.html\">terminate a process </a>");
        document.write("<p><A href=\"stub.html\">actions </a>");
    }
    if (userType==3 || userType==1) {
        document.write("<p><A href=\"stub.html\">monitor a process </a>");
        document.write("<p><A href=\"stub.html\">actions </a>");
    }

    document.write("<p><A href=\"logout.html\">Log out</a>");
}
