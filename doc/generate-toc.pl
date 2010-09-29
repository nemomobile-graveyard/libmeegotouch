#!/usr/bin/perl -w

use strict;
use utf8;
use Encode;
use English;

binmode STDIN,  ":encoding(utf8)";
binmode STDOUT, ":encoding(utf8)";

my $level = -1;

while (<>) {
    if ($ARG =~ /^[[:space:]]*\\(sub)*section[[:space:]]+([^[:space:]]+)[[:space:]]+(.*)$/) {
        my $sectionReference = $2;
        my $sectionTitle = $3;
        my $newlevel = $level;
        if ($ARG =~ /\\section/) {
            $newlevel = 0;
        }
        elsif ($ARG =~ /\\subsection/) {
            $newlevel = 1;
        }
        elsif ($ARG =~ /\\subsubsection/) {
            $newlevel = 2;
        }
        elsif ($ARG =~ /\\subsubsubsection/) {
            $newlevel = 3;
        }
        elsif ($ARG =~ /\\subsubsubsubsection/) {
            $newlevel = 4;
        }
        elsif ($ARG =~ /\\subsubsubsubsubsection/) {
            $newlevel = 5;
        }
        else {
            print "too many levels. Exit\n";
            exit (1);
        }
        while ($newlevel > $level) {
            print "<ul>\n";
            $level++;
        }
        while ($newlevel < $level) {
            print "</ul>\n";
            $level--;
        }
        print "<li><a href=\"#$sectionReference\">$sectionTitle</a></li>\n";
    }
}
while ($level > -1) {
    print "</ul>\n";
    $level--;
}
