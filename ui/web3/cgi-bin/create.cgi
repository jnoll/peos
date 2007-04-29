#!/usr/bin/perl
# $Id: create.cgi,v 1.3 2007/04/29 18:54:27 jnoll Exp $  -*-perl-*-
# Create process instance.

use CGI qw/:standard/;

print header;
print start_html('create process');
print h1('I Hate Everybody');
# Get form variables.
param;
my $model = param('model');

print start_pre; print "model: $model\n"; print end_pre;

print start_pre;


if (-r "${model}.pml" ) {
    print "executing cmd\n";
    $result = `./peos -c ${model}.pml 2>&1`;
}

$content=`/usr/bin/xsltproc list_procs.xsl proc_table.dat.xml 2>&1`;
print end_pre;

print $content;

print start_pre;
print "model: $model result: $result\n";
print end_pre;

print end_html;
