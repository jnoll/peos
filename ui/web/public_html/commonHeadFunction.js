var origWidth = window.innerWidth;
var origHeight = window.innerHeight;

function reloadPage() {//Fixes funky netscape behavior when window is resized
    if (navigator.appName=="Netscape") {
        if (origWidth != window.innerWidth || origHeight != window.innerHeight) {
            location.reload();
        }
    }
}

// functions to read cookies from User (GetCookie() & getCookieVal() must be used together!)
function getCookie(name) {
    var arg = name + "=";
    var alen = arg.length;
    var clen = document.cookie.length;
    var i=0;
    while (i < clen) {
        var j = i+alen;

        if (document.cookie.substring(i,j) == arg)
            return getCookieVal(j);
            i = document.cookie.indexOf(" ", i) + 1;
            if (i == 0) break;
        }
    return null;
}

function getCookieVal(offset) {
    var endstr = document.cookie.indexOf (";", offset);
    if (endstr == -1)
        endstr = document.cookie.length;
    return unescape(document.cookie.substring(offset, endstr));
}

