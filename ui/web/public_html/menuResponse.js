function invokeAction(inCommand, inAction){

    document.webuiForm.command.value = inCommand;
    document.webuiForm.action.value = inAction;
    //document.webuiForm.sessionid.value = sessionidVar;
    document.webuiForm.username.value = userName;
    document.webuiForm.password.value = passwordVar;

    alert("command : "   + document.webuiForm.command.value+"\n"+
          "action : "    + document.webuiForm.action.value+"\n"+
          //"sessionid : " + document.webuiForm.sessionid.value+"\n"+
          "username : "  + document.webuiForm.username.value+"\n"+
          "password : "  + document.webuiForm.password.value);


    document.webuiForm.submit();
}

function callCGI(inCommand){
    var nothingChecked = true;
    for(var i = 0; i < document.actionSelection.model_action.length ; i++) {
        if(document.actionSelection.model_action[i].checked){
            nothingChecked = false;
            break;
        }
    }
    if(!nothingChecked){
        invokeAction(inCommand, document.actionSelection.model_action[i].value);
    }
    else{
        alert("please select an item");
    }
}


