#!/usr/bin/perl
# $Id: create.cgi,v 1.4 2007/04/29 19:23:51 jnoll Exp $  -*-perl-*-
# Create a process instance.

use CGI qw(-debug :standard);

# Redirect to process instance list.  This solves two problems:
# 1. Reloading the page doesn't create another instance.
# 2. This script doesn't have to create any content; we just redirect
#    to whatever page seems appropriate as a result page.
print redirect('http://localhost/~jnoll/peos_test/list_procs.cgi');

# Get form variables.
param;
my $model = param('model');

if (-r "${model}.pml" ) {
    $result = `./peos -c ${model}.pml 2>&1`;
}



