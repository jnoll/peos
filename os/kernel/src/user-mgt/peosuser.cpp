#define _USE_XOPEN
#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream.h>
#include <crypt.h>

const char* usage = "USAGE: peosuser username [password]";

void main( int argc, char** argv )
{
    FILE *password_file;
    struct passwd *uinfo;
    char salt[2];
    salt[0] = '.';
    salt[1] = 'l';

    switch (argc) {
        case 1:
            cerr << usage << endl;
            exit(1);
        case 2:
            uinfo->pw_name = argv[1];
            uinfo->pw_passwd = crypt(argv[1], salt);         
            break;
        default:
            uinfo->pw_name = argv[1];
            uinfo->pw_passwd = crypt(argv[2], salt);         
    }

    if((password_file = fopen("../peos_passwd", "a")) == NULL) {
        cerr << "Could not find password file\n";
        exit(1);
    }

    uinfo->pw_uid = 0;
    uinfo->pw_gid = 0;
    uinfo->pw_gecos = "";
    uinfo->pw_dir = "";
    uinfo->pw_shell = "";

    putpwent(uinfo, password_file);
    fclose(password_file);
    cout << "\nUser added.\n";
    exit(0);
}
