function invokeAction(inCommand){

    document.webuiForm.command.value = inCommand;
    document.webuiForm.action.value = "";
    document.webuiForm.sessionid.value = sessionidVar;
    document.webuiForm.username.value = userName;
    document.webuiForm.password.value = passwordVar;

    document.webuiForm.submit();
}

