/*
 * File: palmUI.h
 * Author: Mark Mastroieni
 * Date: 11/1/02
 */

/* takes a string, displays it on the
 * Palm's screen
 * **NOTE: event handler is responsible for
 * clearing the Palm's display as necessary
 * *before* calling the palmEngine function
 * which will call sendUI() - sendUI just 
 * prints messages out.
 */
void sendUI(char * msg_out);
