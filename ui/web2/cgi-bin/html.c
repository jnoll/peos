#include <stdio.h>

void print_header(char *title)
{
    printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
    printf("<html>\n");
    printf("<head>\n");
    printf("  <meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\n");
    printf("  <title>%s</title>\n", title);
    printf("</head>\n");
    printf("<body>\n");
}

void print_banner()
{
    printf("<table style=\"text-align: left; background-color: rgb(255, 204, 51); width: 100%;\"\n");
    printf(" border=\"0\" cellspacing=\"0\" cellpadding=\"1\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"width:100px;\">\n");
    printf("        <img style=\"width: 100px; height: 100px;\" alt=\"PEOS Logo\"\n");
    printf("        src=\"../images/peos_logo.jpg\">\n");
    printf("      </td>\n");
    printf("      <td style=\"text-align: center; vertical-align: middle;\">\n");
    printf("        <h1>PEOS Process Enactment Demonstration</h1>\n");
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
}

void print_footer()
{
    printf("<table style=\"text-align: left; width: 100%;\" border=\"0\" cellspacing=\"0\"\n");
    printf(" cellpadding=\"1\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"background-color: rgb(255, 204, 51);\">\n");
    printf("      &copy;2004 Santa Clara University. Please direct your inquiries:\n");
    printf("      <a href=\"mailto:jnoll@scu.edu\">jnoll@scu.edu</a>\n");
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");
}
