#/***************************************************************************
#**
#** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
#** All rights reserved.
#** Contact: Nokia Corporation (directui@nokia.com)
#**
#** This file is part of libmeegotouch.
#**
#** If you have questions regarding the use of this file, please contact
#** Nokia at directui@nokia.com.
#**
#** This library is free software; you can redistribute it and/or
#** modify it under the terms of the GNU Lesser General Public
#** License version 2.1 as published by the Free Software Foundation
#** and appearing in the file LICENSE.LGPL included in the packaging
#** of this file.
#**
#****************************************************************************/
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
