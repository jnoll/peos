function deleteCookie () {
    thisCookie = document.cookie.split("; ");
    for(i=0; i<thisCookie.length; i++) {
        cookieName = thisCookie[i].split("=")[0];
        document.cookie = cookieName + "=;expires=Thu, 01-Jan-70 00:00:01 GMT";
    }
    window.location="Login.html";
}

if(getCookie("userName") == null){
    window.location="Login.html";
}

