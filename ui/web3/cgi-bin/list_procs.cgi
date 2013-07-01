#!/usr/bin/perl
# $Id: list_procs.cgi,v 1.3 2007/04/29 18:54:27 jnoll Exp $  -*-perl-*-
# List process instances.

use CGI qw/:standard/;

print header;
print start_html('Process Instances');
print h1('Process Instances');


$content=`/usr/bin/xsltproc list_procs.xsl proc_table.dat.xml 2>&1`;
$result=$?;
print $content;

print start_pre;
print "model: $model result: $result\n";
print end_pre;

print end_html;
