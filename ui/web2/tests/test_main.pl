#!/usr/local/bin/perl

`../cgi-bin/main.cgi > output`;

if(`grep Status output` eq "")
{
  print "Failed status.\n";
}
if(`grep Task output` eq "")
{
  print "Failed task.\n";
}
if(`grep Resource\\(s\\) output` eq "")
{
  print "Failed resource.\n";
}
if(`grep Description output` eq "")
{
  print "Failed description.\n";
}
if(`grep ready output` eq "")
{
  print "Failed ready.\n";
}

